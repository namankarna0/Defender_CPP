document.addEventListener("DOMContentLoaded", () => {
  const input=document.getElementById("encryptedMessage"), password=document.getElementById("decryptionPassword"), output=document.getElementById("decryptedText");
  document.getElementById("showDecryptionPassword").onclick=()=>password.type=password.type==="password"?"text":"password";
  document.getElementById("decryptTextButton").onclick=async()=>{ if(!input.value||!password.value)return alert("Enter encrypted text and a password."); try { const response=await fetch("/api/decrypt/text",{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify({text:input.value,password:password.value})}); const data=await response.json(); if(!response.ok)throw new Error(data.error); output.value=data.text; } catch(error){alert(error.message||"Decryption failed.");} };
});
