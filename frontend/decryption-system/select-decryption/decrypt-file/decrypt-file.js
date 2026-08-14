document.addEventListener("DOMContentLoaded", () => {

    const fileInput =
        document.getElementById("decryptFile");

    const selectButton =
        document.getElementById("selectDecryptFile");

    const fileName =
        document.getElementById("decryptFileName");

    const password =
        document.getElementById("decryptFilePassword");

    const decryptButton =
        document.getElementById("startDecryption");

    const successMessage =
        document.getElementById("decryptionSuccess");

    const errorMessage =
        document.getElementById("decryptionError");

    const errorText =
        document.getElementById("decryptionErrorText");


    /* Select file */

    selectButton.addEventListener("click", () => {

        fileInput.click();

    });


    /* Display selected file */

    fileInput.addEventListener("change", () => {

        successMessage.classList.remove("show");
        errorMessage.classList.remove("show");


        if (fileInput.files.length > 0) {

            fileName.textContent =
                fileInput.files[0].name;

        }
        else {

            fileName.textContent =
                "No file selected";

        }

    });


    /* Start decryption */

    decryptButton.addEventListener("click", async () => {

        successMessage.classList.remove("show");
        errorMessage.classList.remove("show");


        if (fileInput.files.length === 0) {

            errorText.textContent =
                "Please select an encrypted file first.";

            errorMessage.classList.add("show");

            return;
        }


        if (password.value.trim() === "") {

            errorText.textContent =
                "Please enter the decryption password.";

            errorMessage.classList.add("show");

            return;
        }


        try {
            const formData = new FormData(); formData.append("file", fileInput.files[0]); formData.append("password", password.value);
            const response = await fetch("/api/decrypt/file", { method: "POST", body: formData });
            if (!response.ok) { const data = await response.json(); throw new Error(data.error); }
            const blob = await response.blob(); const link = document.createElement("a"); link.href = URL.createObjectURL(blob); link.download = fileInput.files[0].name.replace(/\.enc$/, "") || "decrypted-file"; link.click(); URL.revokeObjectURL(link.href);
            successMessage.classList.add("show");
        } catch (error) { errorText.textContent = error.message || "Decryption failed."; errorMessage.classList.add("show"); }

    });

});
