// Sends simulation inputs to the backend and renders its calculated results.
document.addEventListener("DOMContentLoaded", () => {
  const targetPassword = document.getElementById("targetPassword");
  const showPassword = document.getElementById("showPassword");
  const startSimulation = document.getElementById("startSimulation");

  // Eye Button Toggle Listener
  if (showPassword && targetPassword) {
    showPassword.addEventListener("click", () => {
      // Toggle input type between "password" and "text"
      const isPassword = targetPassword.type === "password";
      targetPassword.type = isPassword ? "text" : "password";

      // Toggle FontAwesome regular icon classes (fa-eye <-> fa-eye-slash)
      const icon = showPassword.querySelector("i");
      if (icon) {
        icon.classList.toggle("fa-eye");
        icon.classList.toggle("fa-eye-slash");
      }
    });
  }

  // Simulation Logic
  startSimulation.onclick = async () => {
    const password = targetPassword.value,
      speed = Number(document.getElementById("attackSpeed").value);

    if (!password || !speed)
      return alert("Enter a password and positive attack speed.");

    try {
      const response = await fetch("/api/password/attack-simulate", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ password, attempts_per_second: speed }),
      });

      const data = await response.json();
      if (!response.ok) throw new Error(data.error);

      document.getElementById("combinationResult").textContent =
        data.total_combinations;
      document.getElementById(
        "speedResult"
      ).textContent = `${speed.toLocaleString()}/sec`;
      document.getElementById("timeResult").textContent = data.common_password
        ? `${data.attack_seconds.toFixed(2)} seconds (${data.dictionary_position} dictionary attempts)`
        : `10^${data.attack_log10_seconds.toFixed(2)} seconds`;
    } catch (error) {
      alert(error.message || "Simulation failed.");
    }
  };
});