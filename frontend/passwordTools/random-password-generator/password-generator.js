document.addEventListener("DOMContentLoaded", () => {
  const byId = id => document.getElementById(id);
  const generate = async () => {
    const payload={length:Number(byId("passwordLength").value),lowercase:byId("lowercase").checked,uppercase:byId("uppercase").checked,numbers:byId("numbers").checked,symbols:byId("symbols").checked};
    try { const response=await fetch("/api/password/generate",{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify(payload)}); const data=await response.json(); if(!response.ok) throw new Error(data.error); byId("generatedPassword").value=data.password; byId("passwordStatus").textContent="Password generated securely."; } catch(error) { byId("passwordStatus").textContent=error.message; }
  };
  byId("generatePasswordBtn").onclick=generate; byId("generateAgainBtn").onclick=generate;
  byId("clearPasswordBtn").onclick=()=>{byId("generatedPassword").value="";byId("passwordStatus").textContent="";};
  byId("copyPasswordBtn").onclick=async()=>{if(byId("generatedPassword").value){await navigator.clipboard.writeText(byId("generatedPassword").value);byId("passwordStatus").textContent="Copied to clipboard.";}};
});
