/// @description Insert description here
// You can write your code in this editor


if(is_moving == true) {
	x++;
}

if(keyboard_check(ord("D"))) {
	is_moving = true;
	desired_position = x + 64
}

if(x == desired_position) {
	is_moving = false;
}

if(keyboard_check(ord("X"))) {
	is_moving = false;
}