document.addEventListener("DOMContentLoaded", () => {
    // DOM Elements
    const fileInput = document.getElementById("encryptFile");
    const selectButton = document.getElementById("selectEncryptFile");
    const fileName = document.getElementById("encryptFileName");
    const passwordInput = document.getElementById("encryptFilePassword");
    const showPasswordBtn = document.getElementById("showEncryptionPassword");
    const encryptButton = document.getElementById("startEncryption");
    const successMessage = document.getElementById("encryptionSuccess");
    const errorMessage = document.getElementById("encryptionError");
    const errorText = document.getElementById("encryptionErrorText");

    /* ==========================================
       1. EYE BUTTON (TOGGLE PASSWORD VISIBILITY)
    ========================================== */
    if (showPasswordBtn && passwordInput) {
        showPasswordBtn.addEventListener("click", () => {
            const isPassword = passwordInput.type === "password";
            passwordInput.type = isPassword ? "text" : "password";

            const eyeIcon = showPasswordBtn.querySelector("i");
            if (eyeIcon) {
                eyeIcon.classList.toggle("fa-eye", !isPassword);
                eyeIcon.classList.toggle("fa-eye-slash", isPassword);
            }

            const titleText = isPassword ? "Hide password" : "Show password";
            showPasswordBtn.setAttribute("title", titleText);
            showPasswordBtn.setAttribute("aria-label", titleText);
        });
    }

    /* ==========================================
       2. TRIGGER FILE SELECT DIALOG
    ========================================== */
    if (selectButton && fileInput) {
        selectButton.addEventListener("click", () => {
            fileInput.click();
        });
    }

    /* ==========================================
       3. DISPLAY SELECTED FILE NAME
    ========================================== */
    if (fileInput) {
        fileInput.addEventListener("change", () => {
            if (successMessage) successMessage.classList.remove("show");
            if (errorMessage) errorMessage.classList.remove("show");

            if (fileInput.files.length > 0) {
                fileName.textContent = fileInput.files[0].name;
            } else {
                fileName.textContent = "No file selected";
            }
        });
    }

    /* ==========================================
       4. START ENCRYPTION ACTION
    ========================================== */
    if (encryptButton) {
        encryptButton.addEventListener("click", async () => {
            if (successMessage) successMessage.classList.remove("show");
            if (errorMessage) errorMessage.classList.remove("show");

            // Input Validation
            if (!fileInput || fileInput.files.length === 0) {
                if (errorText) errorText.textContent = "Please select a file first.";
                if (errorMessage) errorMessage.classList.add("show");
                return;
            }

            if (!passwordInput || passwordInput.value.trim() === "") {
                if (errorText) errorText.textContent = "Please enter an encryption password.";
                if (errorMessage) errorMessage.classList.add("show");
                return;
            }

            try {
                const formData = new FormData();
                formData.append("file", fileInput.files[0]);
                formData.append("password", passwordInput.value);

                const response = await fetch("/api/encrypt/file", {
                    method: "POST",
                    body: formData
                });

                if (!response.ok) {
                    const data = await response.json();
                    throw new Error(data.error);
                }

                // Download encrypted file
                const blob = await response.blob();
                const link = document.createElement("a");
                link.href = URL.createObjectURL(blob);
                link.download = `${fileInput.files[0].name}.enc`;
                link.click();
                URL.revokeObjectURL(link.href);

                if (successMessage) successMessage.classList.add("show");

            } catch (error) {
                if (errorText) errorText.textContent = error.message || "Encryption failed.";
                if (errorMessage) errorMessage.classList.add("show");
            }
        });
    }
});