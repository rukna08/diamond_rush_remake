
function move(){
	if(is_moving == true) {
		switch(move_direction) {
			case "left": 
				x -= 1 * move_speed;
				break;
			case "right":
				x += 1 * move_speed;
				break;
		}
	}
	
	if(keyboard_check(ord("A"))) {
		is_moving = true;
		desired_position = x - 64;
		move_direction = "left";
	} else if(keyboard_check(ord("D"))) {
		is_moving = true;
		desired_position = x + 64;
		move_direction = "right";
	}
	
	// Change ==
	if(x == desired_position) {
		is_moving = false;
	}
}