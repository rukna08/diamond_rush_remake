
function move(){
	if(is_moving_x == true) {
		switch(move_direction) {
			case "left": 
				x -= 1 * move_speed;
				break;
			case "right":
				x += 1 * move_speed;
				break;
		}
	} else if(is_moving_y == true) {
		switch(move_direction) {
			case "up":
				y -= 1 * move_speed;
				break;
			case "down":
				y += 1 * move_speed;
				break;
		}
	}
	
	if(keyboard_check_pressed(ord("A"))) {
		is_moving_x = true;
		desired_position_x = x - 64;
		move_direction = "left";
	} else if(keyboard_check_pressed(ord("D"))) {
		is_moving_x = true;
		desired_position_x = x + 64;
		move_direction = "right";
	}
	
	if(keyboard_check_pressed(ord("W"))) {
		is_moving_y = true;
		desired_position_y = y - 64;
		move_direction = "up";
	} else if(keyboard_check_pressed(ord("S"))) {
		is_moving_y = true;
		desired_position_y = y + 64;
		move_direction = "down";
	}
	
	if(x == desired_position_x) {
		is_moving_x = false;
	}
	
	if(y == desired_position_y) {
		is_moving_y = false;
	}
}