var i = 0;

function update() {
    i++;

    document.getElementById("result").innerText = i;
}

document.getElementById("origin").addEventListener("change", update);