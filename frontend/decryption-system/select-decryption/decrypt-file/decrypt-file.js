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

    decryptButton.addEventListener("click", () => {

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


        /*
         * BACKEND WILL BE CONNECTED HERE
         *
         * Later:
         *
         * const formData = new FormData();
         * formData.append("file", fileInput.files[0]);
         * formData.append("password", password.value);
         *
         * fetch("http://localhost:8080/decrypt-file", {
         *     method: "POST",
         *     body: formData
         * });
         */


        successMessage.classList.add("show");

    });

});