function makeChoice(i) {
    if (i == 0) {
        document.getElementById("top_restaurant_box").style.display = "block";
        document.getElementById("other_restaurants_box").style.display = "none";

        document.getElementById("other_res_choice").style.borderWidth = "0px";
	document.getElementById("other_res_choice").style.background = "0";
	document.getElementById("top_res_choice").style.background = "rgba(32, 25, 36, 0.3)";
        document.getElementById("top_res_choice").style.borderWidth = "3px";
    } else {
        document.getElementById("top_restaurant_box").style.display = "none";
        document.getElementById("other_restaurants_box").style.display = "block";
    
        document.getElementById("other_res_choice").style.borderWidth = "3px";
	document.getElementById("other_res_choice").style.background = "rgba(32, 25, 36, 0.3)";
	document.getElementById("top_res_choice").style.background = "0";
	document.getElementById("top_res_choice").style.borderWidth = "0px";
    }
}

function changePhotos(i, n) {
    var r = 4 * (i + 1);
    if (r >= n) {
        var next = 0;
    } else {
        var next = r / 4;
    }

    document.getElementById("top_restaurant_pictures_" + i).style.display = "none";    
    document.getElementById("top_restaurant_pictures_" + next).style.display = "block";
}
