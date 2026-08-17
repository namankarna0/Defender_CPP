document.addEventListener("DOMContentLoaded", () => {
  const input = document.getElementById("encryptedMessage");
  const password = document.getElementById("decryptionPassword");
  const output = document.getElementById("decryptedText");
  const showPasswordBtn = document.getElementById("showDecryptionPassword");
  const copyBtn = document.getElementById("copyDecryptedText");
  const decryptBtn = document.getElementById("decryptTextButton");

  // 1. Eye Button (Password Visibility Toggle & Icon Swap)
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

  // 2. Copy Decrypted Text Button
  if (copyBtn && output) {
    copyBtn.addEventListener("click", async () => {
      if (!output.value) {
        alert("No decrypted text to copy.");
        return;
      }

      try {
        await navigator.clipboard.writeText(output.value);

        // Visual feedback: Switch copy icon to checkmark for 2 seconds
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

  // 3. Decrypt Action
  if (decryptBtn) {
    decryptBtn.addEventListener("click", async () => {
      if (!input.value || !password.value) {
        return alert("Enter encrypted text and a password.");
      }

      try {
        const response = await fetch("/api/decrypt/text", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({
            text: input.value,
            password: password.value,
          }),
        });

        const data = await response.json();
        if (!response.ok) throw new Error(data.error);

        output.value = data.text;
      } catch (error) {
        alert(error.message || "Decryption failed.");
      }
    });
  }
});