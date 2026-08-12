document.addEventListener("DOMContentLoaded", () => {

    const fileInput =
        document.getElementById("encryptFile");

    const selectButton =
        document.getElementById("selectEncryptFile");

    const fileName =
        document.getElementById("encryptFileName");

    const password =
        document.getElementById("encryptFilePassword");

    const encryptButton =
        document.getElementById("startEncryption");

    const successMessage =
        document.getElementById("encryptionSuccess");

    const errorMessage =
        document.getElementById("encryptionError");

    const errorText =
        document.getElementById("encryptionErrorText");


    /* Select file */

    selectButton.addEventListener("click", () => {

        fileInput.click();

    });


    /* Show selected file */

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


    /* Start encryption */

    encryptButton.addEventListener("click", () => {

        successMessage.classList.remove("show");
        errorMessage.classList.remove("show");


        if (fileInput.files.length === 0) {

            errorText.textContent =
                "Please select a file first.";

            errorMessage.classList.add("show");

            return;
        }


        if (password.value.trim() === "") {

            errorText.textContent =
                "Please enter an encryption password.";

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
         * fetch("http://localhost:8080/encrypt-file", {
         *     method: "POST",
         *     body: formData
         * });
         */


        successMessage.classList.add("show");

    });

});