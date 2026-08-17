document.addEventListener("DOMContentLoaded", () => {
  const input = document.getElementById("plainText");
  const password = document.getElementById("encryptionPassword");
  const output = document.getElementById("encryptedText");
  const showPasswordBtn = document.getElementById("showEncryptionPassword");
  const copyBtn = document.getElementById("copyEncryptedText");
  const encryptBtn = document.getElementById("encryptTextButton");

  // 1. Password Visibility Toggle (Eye Button)
  if (showPasswordBtn && password) {
    showPasswordBtn.addEventListener("click", () => {
      const isPassword = password.type === "password";
      password.type = isPassword ? "text" : "password";

      const icon = showPasswordBtn.querySelector("i");
      if (icon) {
        icon.classList.toggle("fa-eye");
        icon.classList.toggle("fa-eye-slash");
      }
    });
  }

  // 2. Copy Encrypted Message Button
  if (copyBtn && output) {
    copyBtn.addEventListener("click", async () => {
      if (!output.value) {
        alert("No encrypted text to copy.");
        return;
      }

      try {
        await navigator.clipboard.writeText(output.value);

        // Visual Feedback: Temporarily change icon to a checkmark
        const icon = copyBtn.querySelector("i");
        if (icon) {
          icon.className = "fa-solid fa-check";
          setTimeout(() => {
            icon.className = "fa-regular fa-copy";
          }, 2000);
        }
      } catch (err) {
        alert("Failed to copy text.");
      }
    });
  }

  // 3. Encrypt Action
  if (encryptBtn) {
    encryptBtn.addEventListener("click", async () => {
      if (!input.value || !password.value) {
        return alert("Enter text and an encryption password.");
      }

      try {
        const response = await fetch("/api/encrypt/text", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({
            text: input.value,
            password: password.value,
          }),
        });

        const data = await response.json();
        if (!response.ok) throw new Error(data.error);

        output.value = data.encrypted;
      } catch (error) {
        alert(error.message || "Encryption failed.");
      }
    });
  }
});