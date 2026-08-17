document.addEventListener("DOMContentLoaded", () => {
  const password = document.getElementById("password"),
    togglePassword = document.getElementById("togglePassword"),
    button = document.getElementById("analyzeButton");

  // Eye Button Toggle Listener
  togglePassword.addEventListener("click", () => {
    // Toggle input type between "password" and "text"
    const isPassword = password.getAttribute("type") === "password";
    password.setAttribute("type", isPassword ? "text" : "password");

    // Toggle icon and aria-label
    const icon = togglePassword.querySelector("i");
    icon.classList.toggle("fa-eye");
    icon.classList.toggle("fa-eye-slash");
    
    togglePassword.setAttribute(
      "aria-label",
      isPassword ? "Hide password" : "Show password"
    );
  });

  // Password Analysis Listener
  button.onclick = async () => {
    if (!password.value) {
      document.getElementById("error").textContent =
        "Enter a password to analyze.";
      return;
    }
    try {
      const response = await fetch("/api/password/check", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ password: password.value }),
      });
      const data = await response.json();
      if (!response.ok) throw new Error(data.error);
      const yes = (value) => (value ? "Yes" : "No");
      document.getElementById("error").textContent = "";
      document.getElementById("strength").textContent = data.strength;
      document.getElementById("score").textContent = data.score;
      document.getElementById("length").textContent = data.length;
      document.getElementById("uppercase").textContent = yes(data.uppercase);
      document.getElementById("lowercase").textContent = yes(data.lowercase);
      document.getElementById("digit").textContent = yes(data.digit);
      document.getElementById("symbol").textContent = yes(data.symbol);
      document.getElementById("repeated").textContent = yes(data.repeated);
      document.getElementById("commonpassword").textContent =
        yes(data.common_password);
      document.getElementById("strengthBar").style.width = `${data.score}%`;
      document.getElementById("result").classList.remove("hidden");
    } catch (error) {
      document.getElementById("error").textContent =
        error.message || "Analysis failed.";
    }
  };
});