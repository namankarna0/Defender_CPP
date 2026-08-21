document.addEventListener("DOMContentLoaded", () => {
    // DOM Elements
    const fileInput = document.getElementById("decryptFile");
    const selectButton = document.getElementById("selectDecryptFile");
    const fileName = document.getElementById("decryptFileName");
    const passwordInput = document.getElementById("decryptFilePassword");
    const showPasswordBtn = document.getElementById("showDecryptionPassword");
    const decryptButton = document.getElementById("startDecryption");
    const successMessage = document.getElementById("decryptionSuccess");
    const errorMessage = document.getElementById("decryptionError");
    const errorText = document.getElementById("decryptionErrorText");

    /* ==========================================
       1. EYE BUTTON (TOGGLE PASSWORD VISIBILITY)
    ========================================== */
    if (showPasswordBtn && passwordInput) {
        showPasswordBtn.addEventListener("click", () => {
            // Check current type
            const isPassword = passwordInput.type === "password";
            
            // Toggle input type
            passwordInput.type = isPassword ? "text" : "password";

            // Toggle Font Awesome icon classes
            const icon = showPasswordBtn.querySelector("i");
            if (icon) {
                icon.classList.toggle("fa-eye", !isPassword);
                icon.classList.toggle("fa-eye-slash", isPassword);
            }
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
       3. UPDATE FILE NAME DISPLAY
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
       4. HANDLE FILE DECRYPTION
    ========================================== */
    if (decryptButton) {
        decryptButton.addEventListener("click", async () => {
            // Clear status banners
            if (successMessage) successMessage.classList.remove("show");
            if (errorMessage) errorMessage.classList.remove("show");

            // Input Validation
            if (!fileInput || fileInput.files.length === 0) {
                if (errorText) errorText.textContent = "Please select an encrypted file first.";
                if (errorMessage) errorMessage.classList.add("show");
                return;
            }

            if (!passwordInput || passwordInput.value.trim() === "") {
                if (errorText) errorText.textContent = "Please enter the decryption password.";
                if (errorMessage) errorMessage.classList.add("show");
                return;
            }

            try {
                const formData = new FormData();
                formData.append("file", fileInput.files[0]);
                formData.append("password", passwordInput.value);

                const response = await fetch("/api/decrypt/file", {
                    method: "POST",
                    body: formData
                });

                if (!response.ok) {
                    const data = await response.json();
                    throw new Error(data.error);
                }

                // Download decrypted file
                const blob = await response.blob();
                const link = document.createElement("a");
                link.href = URL.createObjectURL(blob);
                link.download = fileInput.files[0].name.replace(/\.enc$/, "") || "decrypted-file";
                link.click();
                URL.revokeObjectURL(link.href);

                // Show success message
                if (successMessage) successMessage.classList.add("show");

            } catch (error) {
                if (errorText) errorText.textContent = error.message || "Decryption failed.";
                if (errorMessage) errorMessage.classList.add("show");
            }
        });
    }
});