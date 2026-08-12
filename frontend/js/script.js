/*==================================================
    DEFENDER++
    Main JavaScript File
==================================================*/


// ==================================================
// PAGE FADE IN ANIMATION
// ==================================================

document.addEventListener("DOMContentLoaded", () => {

    document.body.classList.add("page-loaded");


});


// ==================================================
// NAVBAR SCROLL EFFECT
// ==================================================

const header = document.querySelector("header");


window.addEventListener("scroll", () => {

    if(window.scrollY > 50){

        header.classList.add("scrolled");

    }
    else{

        header.classList.remove("scrolled");

    }

});


// ==================================================
// SCROLL REVEAL ANIMATION
// ==================================================

const revealElements = document.querySelectorAll(
    ".card, .generatorCard, .title, .pageHero"
);


const revealObserver = new IntersectionObserver(

(entries)=>{

    entries.forEach(entry=>{

        if(entry.isIntersecting){

            entry.target.classList.add("show");

        }

    });

},

{

threshold:0.15

}

);



revealElements.forEach(element=>{

    element.classList.add("hidden");

    revealObserver.observe(element);

});


// ==================================================
// BUTTON RIPPLE EFFECT
// ==================================================

const buttons = document.querySelectorAll(".btn, .card a");


buttons.forEach(button=>{


    button.addEventListener("click",(e)=>{


        let ripple = document.createElement("span");


        ripple.classList.add("ripple");


        let rect = button.getBoundingClientRect();


        let x = e.clientX - rect.left;

        let y = e.clientY - rect.top;


        ripple.style.left = `${x}px`;

        ripple.style.top = `${y}px`;


        button.appendChild(ripple);


        setTimeout(()=>{

            ripple.remove();

        },600);


    });


});


// ==================================================
// MOUSE GLOW EFFECT
// ==================================================

const glow = document.createElement("div");


glow.className="mouse-glow";


document.body.appendChild(glow);



document.addEventListener("mousemove",(e)=>{


    glow.style.left = e.clientX + "px";

    glow.style.top = e.clientY + "px";


});


// ==================================================
// SMOOTH PAGE TRANSITION
// ==================================================

const pageLinks = document.querySelectorAll(
    "a[href$='.html']"
);


pageLinks.forEach(link=>{


    link.addEventListener("click",(e)=>{


        let url = link.href;


        if(url.includes("#")) return;


        e.preventDefault();


        document.body.classList.add("page-exit");


        setTimeout(()=>{


            window.location.href=url;


        },400);


    });


});


// ==================================================
// ACTIVE NAV LINK
// ==================================================

const currentPage =
window.location.pathname.split("/").pop();



document.querySelectorAll("nav a")
.forEach(link=>{


    if(link.getAttribute("href") === currentPage){


        link.classList.add("active");


    }


});


// ==================================================
// CARD TILT EFFECT
// ==================================================

const cards = document.querySelectorAll(".card");


cards.forEach(card=>{


    card.addEventListener("mousemove",(e)=>{


        const rect =
        card.getBoundingClientRect();


        const x =
        e.clientX - rect.left;


        const y =
        e.clientY - rect.top;


        const rotateX =
        (y - rect.height/2)/15;


        const rotateY =
        (x - rect.width/2)/15;



        card.style.transform =
        `
        perspective(700px)
        rotateX(${-rotateX}deg)
        rotateY(${rotateY}deg)
        translateY(-10px)
        `;


    });



    card.addEventListener("mouseleave",()=>{


        card.style.transform="";


    });


});


// ==================================================
// TYPING EFFECT FOR HERO TITLE (OPTIONAL)
// ==================================================

const heroTitle =
document.querySelector(".hero h1");


if(heroTitle){


    const text =
    heroTitle.innerText;


    heroTitle.innerText="";


    let index=0;


    function typeEffect(){


        if(index < text.length){


            heroTitle.innerHTML += text[index];


            index++;


            setTimeout(typeEffect,120);


        }


    }


    typeEffect();


}
/* ==================================================
   DEFENDER++
   Password Strength & Breach Checker
   FRONTEND ONLY
================================================== */

document.addEventListener("DOMContentLoaded", () => {

    const passwordInput =
        document.getElementById("password");

    const togglePassword =
        document.getElementById("togglePassword");

    const analyzeButton =
        document.getElementById("analyzeButton");

    const error =
        document.getElementById("error");

    const result =
        document.getElementById("result");

    const strength =
        document.getElementById("strength");

    const score =
        document.getElementById("score");

    const strengthBar =
        document.getElementById("strengthBar");


    /* ==============================================
       SHOW / HIDE PASSWORD
    ============================================== */

    togglePassword.addEventListener("click", () => {

        const isPassword =
            passwordInput.type === "password";

        passwordInput.type =
            isPassword ? "text" : "password";

        togglePassword.innerHTML = isPassword
            ? '<i class="fa-solid fa-eye-slash"></i>'
            : '<i class="fa-solid fa-eye"></i>';

    });


    /* ==============================================
       FRONTEND PLACEHOLDER

       No backend request is made here.
       C++ integration can be added later.
    ============================================== */

    analyzeButton.addEventListener("click", () => {

        error.textContent = "";

        const password =
            passwordInput.value;

        if (!password) {

            result.classList.add("hidden");

            error.textContent =
                "Please enter a password.";

            return;

        }


        /*
            FRONTEND DEMO ONLY

            This creates visual placeholder
            information so you can see the UI.

            Replace this section later with
            your C++ backend response.
        */

        const hasUppercase =
            /[A-Z]/.test(password);

        const hasLowercase =
            /[a-z]/.test(password);

        const hasDigit =
            /[0-9]/.test(password);

        const hasSymbol =
            /[^A-Za-z0-9]/.test(password);

        const hasRepeated =
            /(.)\1/.test(password);


        let points = 0;

        points += Math.min(password.length, 8);

        if (hasUppercase) points++;

        if (hasLowercase) points++;

        if (hasDigit) points++;

        if (hasSymbol) points++;

        if (!hasRepeated && password.length > 0) {
            points++;
        }


        points =
            Math.min(points, 14);


        let strengthText;


        if (points <= 4) {

            strengthText = "Weak";

        }
        else if (points <= 8) {

            strengthText = "Moderate";

        }
        else if (points <= 11) {

            strengthText = "Strong";

        }
        else {

            strengthText = "Very Strong";

        }


        /* ==========================================
           DISPLAY VALUES
        ========================================== */

        document.getElementById("length")
            .textContent = password.length;

        document.getElementById("uppercase")
            .textContent =
            hasUppercase ? "Yes" : "No";

        document.getElementById("lowercase")
            .textContent =
            hasLowercase ? "Yes" : "No";

        document.getElementById("digit")
            .textContent =
            hasDigit ? "Yes" : "No";

        document.getElementById("symbol")
            .textContent =
            hasSymbol ? "Yes" : "No";

        document.getElementById("repeated")
            .textContent =
            hasRepeated ? "Yes" : "No";

        document.getElementById("commonpassword")
            .textContent = "Pending";


        strength.textContent =
            strengthText;

        score.textContent =
            points;


        /* ==========================================
           STRENGTH BAR
        ========================================== */

        const percentage =
            (points / 14) * 100;

        strengthBar.style.width =
            percentage + "%";


        /* ==========================================
           SHOW RESULT
        ========================================== */

        result.classList.remove("hidden");

    });

});