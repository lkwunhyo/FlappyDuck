/* FLAPPY DUCK
 * An imitation of the well-known Flappy Bird mobile game. The player 
 * controls a duck to jump to avoid a series of pipes moving towards 
 * the player. The game increases in difficulty by slowly incrementing 
 * the speed of the pipes. 
 *
 * Best played in 115x45 resolution.
 */
 
 // Library Includes
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

// Configuration
#define DELAY (10) /* Millisecond delay between game updates */
#define BIRD_WIDTH (6)
#define BIRD_HEIGHT (3)
#define PIPE_WIDTH (4)

#define KEY_UP		0403		/* up-arrow key */

void display_status( void );

// Game states
bool game_over = true;
bool update_screen = true;
bool score_flag = false;

// Game status information
time_t seconds = 0;
time_t minutes = 0;
time_t time = 0;

int score = 0;

// Game Sprite Images
char * bird_image =
/**/	"   _  "
/**/	"__(0)<"
/**/	"\\___) ";

char * birdfall_image = 
/**/	"   _  "
/**/	"\\\\(0)<"
/**/	"\\___) ";
;

char * birdjump_image = 
/**/	"   _  "
/**/	"  (0)<"
/**/	"//__) ";

char * pipe_img =
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"===="
/**/	"====";

char * plat_image = 
/**/	"=============================================================================================================================================================================================================================================";

char * top_plat_image = 
/**/	"_____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________";

sprite_id logo;

sprite_id ground;
sprite_id status_bar;
sprite_id bird;
sprite_id pipe1_1;
sprite_id pipe1_2;
sprite_id pipe2_1;
sprite_id pipe2_2;

// Setup up game variables and objects.
void setup( void ) {	
	// Set up ground position variables.
	int ground_x;
	int ground_y;
	
	ground_x = 0;
	ground_y = screen_height() - 1;
	
	// Initialise ground sprite
	ground = sprite_create(ground_x, ground_y, screen_width(), 1, plat_image);
	
	// Set up bird initial position.
	int bird_x, bird_y;
	
	bird_x = screen_width() / 10;	// Position from the left
	bird_y = (screen_height() - 1) / 3;
	
	bird = sprite_create(bird_x, bird_y, BIRD_WIDTH, BIRD_HEIGHT, bird_image);
}

// Collision detection.
bool sprites_collided(sprite_id sprite_1, sprite_id sprite_2) {
	// Outer dimensions of sprite_1.
    int sprite_1_top = round(sprite_y(sprite_1)),
        sprite_1_bottom = sprite_1_top + sprite_height(sprite_1) - 1,
        sprite_1_left = round(sprite_x(sprite_1)),
        sprite_1_right = sprite_1_left + sprite_width(sprite_1) - 1;

	// Outer dimensions of sprite_2.
    int sprite_2_top = round(sprite_y(sprite_2)),
        sprite_2_bottom = sprite_2_top + sprite_height(sprite_2) - 1,
        sprite_2_left = round(sprite_x(sprite_2)),
        sprite_2_right = sprite_2_left + sprite_width(sprite_2) - 1;

	// Returns if sprite_1 and sprite_2 has collided.
    return !(
        sprite_1_bottom < sprite_2_top
        || sprite_1_top > sprite_2_bottom
        || sprite_1_right < sprite_2_left
        || sprite_1_left > sprite_2_right
        );
}

// Timer algorithm.
void timer( void ) {
	time++;
	if (time == 100) {
		seconds++;
		time = 0;
		if (seconds == 60) {
			seconds = 0;
			minutes++;
		}
	}
		
}

// Set up the status bar.
void display_status() {
	// Set up status width interval variable.
	int width = screen_width() / 4;
	
	// Drawing status bar.
	int left, top, right, bottom;
	
	left = 0;
	top = 2;
	right = screen_width() - 1;
	bottom = screen_height() - 1;
	
	status_bar = sprite_create(left, top, screen_width(), 1, top_plat_image);
	sprite_draw(status_bar);
	
	// Draw status bar information.
	draw_string(screen_width() / 20, 1, "FLAPPY DUCK");
	draw_formatted(2 * width, 1, "Time: %02d:%02d", minutes, seconds);
	draw_formatted(3 * width, 1, "Score: %d", score);

}

// Set up the start screen bird logo.
void start_screen( void ) {	
	int bird_width, bird_height;

	bird_width = (screen_width() - 6) / 2;
	bird_height = screen_height() / 2;
	
	logo = sprite_create(bird_width, bird_height, 6, 3, bird_image);
	sprite_draw(logo);
}

// Start screen process.
void startscreen_process (void ) {
	sprite_draw(logo);
	sprite_step(logo);
	
	int logo_width, logo_height, text_width, text_height, bird_width, bird_height;
	
	logo_width = (screen_width() - 92) / 2;
	logo_height = screen_height() / 6;
	text_width = (screen_width() - 26) / 2;
	text_height = 3 * screen_height() / 4;
	bird_width = (screen_width() - 6) / 2;
	bird_height = screen_height() / 2;
	
	// Defining physics variables.
	double gravity, scale;
	
	gravity = 9.8;
	scale = 0.00125;
	
	// Defining jump variables.
	double bdy;

	bdy = 0.45;
	
	sprite_turn_to(logo, 0, sprite_dy(logo) + gravity * scale);
	
	// Applying gravity.
	if (sprite_y(logo) > text_height - 5) {
		sprite_turn_to(logo, 0, -bdy);
	}
	
	// Changing sprite image based on jumping or falling.
	if (sprite_dy(logo) > 0 && logo->bitmap != birdfall_image) {
		logo->bitmap = birdfall_image;
	} else if (sprite_dy(logo) < 0 && logo->bitmap != birdjump_image) {
		logo->bitmap = birdjump_image;
	}
	
	draw_string(logo_width, logo_height,	 " _______  ___      _______  _______  _______  __   __    ______   __   __  _______  ___   _ ");
	draw_string(logo_width, logo_height + 1, "|       ||   |    |   _   ||       ||       ||  | |  |  |      | |  | |  ||       ||   | | |");
	draw_string(logo_width, logo_height + 2, "|    ___||   |    |  |_|  ||    _  ||    _  ||  |_|  |  |  _    ||  | |  ||       ||   |_| |");
	draw_string(logo_width, logo_height + 3, "|   |___ |   |    |       ||   |_| ||   |_| ||       |  | | |   ||  |_|  ||       ||      _|");
	draw_string(logo_width, logo_height + 4, "|    ___||   |___ |       ||    ___||    ___||_     _|  | |_|   ||       ||      _||     |_ ");
	draw_string(logo_width, logo_height + 5, "|   |    |       ||   _   ||   |    |   |      |   |    |       ||       ||     |_ |    _  |");
	draw_string(logo_width, logo_height + 6, "|___|    |_______||__| |__||___|    |___|      |___|    |______| |_______||_______||___| |_|");	
	
	draw_string(text_width, text_height, "Press space to continue...");
	
	char key;
	key = get_char();
	
	if (key == ' ') {
		game_over = false;
	}
	
}

// Setting up the game over screen and process.
void gameOver_screen( void ) {
	int text_width, text_height, exittext_width;
	
	text_width = (screen_width() - 32) / 2;
	text_height = (screen_height() - 1) / 2;
	
	exittext_width = (screen_width() - 22) / 2;
	
	draw_formatted(text_width, text_height, "Game Over. Your score was %d ...", score);
	draw_string(exittext_width, text_height + 2, "Press space to exit...");
	
	char key;
	key = get_char();
	
	if (key == ' ') {
		game_over = false;
	}
}

// Setting up the pipes for game initialisation.
void setupPipes( void ) {
	// Pipe position variables.
	int gap, hdist;
	int pipe1_y, pipe2_y;
	double pdx;
	
	pdx = 0.3;
	
	hdist = screen_width() / 2;
	gap = screen_height() / 2;
	
	// Random number generator for deciding the position of the gap in the pipes.
	int max_number, minimum_number;
	max_number = screen_height() - (int)gap - 1 - 1;
	minimum_number = 3;
	
	pipe1_y = rand() % (max_number + 1 - minimum_number) + minimum_number;
	pipe2_y = rand() % (max_number + 1 - minimum_number) + minimum_number;
	
	pipe1_1 = sprite_create(screen_width(), 3, 4, pipe1_y, pipe_img);
	pipe1_2 = sprite_create(screen_width(), pipe1_y + gap, 4, screen_height() - (pipe1_y + gap) - 1, pipe_img);
	
	pipe2_1 = sprite_create(sprite_x(pipe1_1) + hdist, 3, 4, pipe2_y, pipe_img);
	pipe2_2 = sprite_create(sprite_x(pipe1_2) + hdist, pipe2_y + gap, 4, screen_height() - (pipe2_y + gap) - 1, pipe_img);
	
	sprite_turn_to(pipe1_1, -pdx, 0);
	sprite_turn_to(pipe1_2, -pdx, 0);
	
	sprite_turn_to(pipe2_1, -pdx, 0);
	sprite_turn_to(pipe2_2, -pdx, 0);
}

// Generates pipes to appear on the right side of the screen.
void generatePipes( int pipeNo ) {
	// Pipe position variables.
	int gap, hdist;
	int pipe1_y, pipe2_y;
	
	hdist = screen_width() / 2;
	gap = screen_height() / 2;
	
	// Random number generator for deciding the position of the gap in the pipes.
	int max_number, minimum_number;
	max_number = screen_height() - (int)gap - 1 - 1;
	minimum_number = 3;
	
	pipe1_y = rand() % (max_number + 1 - minimum_number) + minimum_number;
	pipe2_y = rand() % (max_number + 1 - minimum_number) + minimum_number;
	
	if (pipeNo == 1) {
		pipe1_1->height = pipe1_y;
		pipe1_2->height = screen_height() - (pipe1_y + gap) - 1;
		sprite_move_to(pipe1_1, screen_width(), 3);
		sprite_move_to(pipe1_2, screen_width(), pipe1_y + gap);
	} else if (pipeNo == 2) {
		pipe2_1->height = pipe2_y;
		pipe2_2->height = screen_height() - (pipe2_y + gap) - 1;
		sprite_move_to(pipe2_1, screen_width(), 3);
		sprite_move_to(pipe2_2, screen_width(), pipe2_y + gap);
	}
}

// Game process.
void process ( void ) {
	// Refreshing the visible bird position on the screen.
	sprite_draw(bird);
	sprite_step(bird);
	
	sprite_draw(ground);
	
	// Refreshing the pipe positions on the screen.
	sprite_draw(pipe1_1);
	sprite_draw(pipe1_2);
	sprite_step(pipe1_1);
	sprite_step(pipe1_2);
	
	sprite_draw(pipe2_1);
	sprite_draw(pipe2_2);
	sprite_step(pipe2_1);
	sprite_step(pipe2_2);
	
	// Defining physics variables.
	double gravity, scale;
	
	gravity = 9.8;
	scale = 0.00125;
	
	// Defining jump variables.
	double bdy;

	bdy = 0.45;
	
	// Defining pipe speed.
	double max_pdx, pdx_inc;

	max_pdx = 1.2;
	pdx_inc = 0.0001;

	
	// Applying gravity.
	sprite_turn_to(bird, sprite_dx(bird), sprite_dy(bird) + gravity * scale);
	if (sprites_collided(bird, status_bar)) {
		sprite_turn_to(bird, 0, 0);
		sprite_move(bird, 0, 1);
	}
	
	// Changing sprite image based on jumping or falling.
	if (sprite_dy(bird) > 0 && bird->bitmap != birdfall_image) {
		bird->bitmap = birdfall_image;
	} else if (sprite_dy(bird) < 0 && bird->bitmap != birdjump_image) {
		bird->bitmap = birdjump_image;
	}
	
	// Detecting collision.
	if (sprites_collided(bird, ground) ||  sprites_collided(bird, pipe1_1) || 
		sprites_collided(bird, pipe1_2) ||	sprites_collided(bird, pipe2_1) || 
		sprites_collided(bird, pipe2_2)) {
		game_over = true;
	}
	
	// Receiving user input.
	int key;
	char keyS;
	 
	keyS = get_char();
	key = get_char();
	
	if (key == KEY_UP || keyS == ' ') {
		// Jump algorithm 
		sprite_turn_to(bird, 0, -bdy);
 
	}
	
	// Moving the pipes towards the bird.
	if (sprite_dx(pipe1_1) > -max_pdx) {
		sprite_turn_to(pipe1_1, sprite_dx(pipe1_1) - pdx_inc, 0);
		sprite_turn_to(pipe1_2, sprite_dx(pipe1_2) - pdx_inc, 0);
		
		sprite_turn_to(pipe2_1, sprite_dx(pipe2_1) - pdx_inc, 0);
		sprite_turn_to(pipe2_2, sprite_dx(pipe2_2) - pdx_inc, 0);
	
	} else {
		sprite_turn_to(pipe1_1, -max_pdx, 0);
		sprite_turn_to(pipe1_2, -max_pdx, 0);
		
		sprite_turn_to(pipe2_1, -max_pdx, 0);
		sprite_turn_to(pipe2_2, -max_pdx, 0);
	}
	
	// Generating new pipes.
	if (sprite_x(pipe1_1) + PIPE_WIDTH < 0) {
		generatePipes(1);
		score_flag = true;
	}
	if (sprite_x(pipe2_1) + PIPE_WIDTH < 0) {
		generatePipes(2);
		score_flag = true;
	}
	 
	// Incrementing the score as the bird passes pipes.
	if (score_flag && (sprite_x(pipe1_1) + 2 < sprite_x(bird) + BIRD_WIDTH / 2 || sprite_x(pipe2_1) + 2 < sprite_x(bird) + BIRD_WIDTH / 2)) {
		if (score_flag) {
			score++;
			score_flag = false;
		}

	}
}

int main( void ) {
	// Setting up the screen.
	setup_screen();
	override_screen_size(115,45);
	
	// Setting up the start screen.
	start_screen();
	
	while(game_over) {
		// Refreshing the screen.
		clear_screen();
		
		startscreen_process();
		
		timer_pause(DELAY);
		show_screen();
	}
	
	// Setting up the game.
	setup();
	setupPipes();
	score_flag = true;
	
	while(!game_over) {
		
		// Refreshing the screen.
		clear_screen();
		
		// Setting up the timer.
		timer();
		
		// Displaying the status bar.
		display_status();
		
		// Update the screen.
		if (update_screen) {
			process();
			show_screen();
		}
		
		timer_pause(DELAY);
		show_screen();
		
	}
	
	// Pause on game_over.
	timer_pause(1000);
	
	// Refreshing the screen.
	clear_screen();
	
	// Setting up the game over screen.
	while(game_over) {
		gameOver_screen();
		show_screen();
	}
	
	return 0;

}
