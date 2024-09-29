
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
	}
	
	if(is_moving_y == true) {
		switch(move_direction) {
			case "up":
				y -= 1 * move_speed;
				break;
			case "down":
				y += 1 * move_speed;
				break;
		}
	}
	
	if(!is_moving_y && (x == desired_position_x || desired_position_x == undefined)) {
		is_moving_x = false;
		if(keyboard_check(ord("A"))) {
			is_moving_x = true;
			desired_position_x = x - 64;
			move_direction = "left";
		} else if(keyboard_check(ord("D"))) {
			is_moving_x = true;
			desired_position_x = x + 64;
			move_direction = "right";
		}		
	}
	
	if(!is_moving_x && (y == desired_position_y || desired_position_y == undefined)) {
		is_moving_y = false;
		if(keyboard_check(ord("W"))) {
			is_moving_y = true;
			desired_position_y = y - 64;
			move_direction = "up";
		} else if(keyboard_check(ord("S"))) {
			is_moving_y = true;
			desired_position_y = y + 64;
			move_direction = "down";
		}
	}
}