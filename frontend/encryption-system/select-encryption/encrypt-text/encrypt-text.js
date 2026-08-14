document.addEventListener("DOMContentLoaded", () => {
  const input = document.getElementById("plainText");
  const password = document.getElementById("encryptionPassword");
  const output = document.getElementById("encryptedText");
  document.getElementById("showEncryptionPassword").onclick = () => password.type = password.type === "password" ? "text" : "password";
  document.getElementById("encryptTextButton").onclick = async () => {
    if (!input.value || !password.value) return alert("Enter text and an encryption password.");
    try { const response = await fetch("/api/encrypt/text", {method:"POST", headers:{"Content-Type":"application/json"}, body:JSON.stringify({text:input.value,password:password.value})}); const data = await response.json(); if (!response.ok) throw new Error(data.error); output.value = data.encrypted; } catch (error) { alert(error.message || "Encryption failed."); }
  };
});
