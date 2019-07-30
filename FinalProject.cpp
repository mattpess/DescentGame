/*
Matthew Pessolano
7/11/2019
CPSC 440
Final Project
FinalProject.cpp
Purpose: Handle intro screen, game loop, block collision, map loading, event triggers, sound effects, and end game screen logic.
*/

#ifndef So
#define So

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include "SpriteSheet.h"

using namespace std;

int collided(int x, int y);  //Tile Collision
bool endValue(int x, int y); // end value is 8
int main(void)
{
	// display dimensions
	const int WIDTH = 900;
	const int HEIGHT = 480;
	// player inputs
	bool keys[] = { false, false, false, false, false };
	enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
	// loop variables
	bool done = false;
	bool render = false;
	//Player Variable
	Sprite player;
	// map errored so made into a variable
	// tracks current map displayed
	int currentMap = 1;
	char map[] = "ENTRANCE.fmp";
	char map2[] = "FLOOR2.fmp";
	char map3[] = "FLOOR3.fmp";
	char map4[] = "FLOOR4.fmp";
	char map5[] = "FLOOR5.fmp";
	// switch counter - which switch pressed
	int curSwitch = 0;
	// money counter
	int money = 0;
	// last text posted; printed to screen every tick
	string text = "Welcome to the dungeon...";
	// number of samples
	int samples = 12;
	// checkpoint tracker - ap2
	int checkpoint = 0;
	// tracking time until next lava block - map4
	int lavaTime = 0;
	// current block taken by lava - map4
	int currentLava = 0;
	// tracks total time spend in cave
	int totalTime = 0;
	// game was force closed
	bool closed = false;
	// stop footstep from playing every tick
	int stepDelay = 0;

	



	//allegro variable
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer;
	// audio clips
	ALLEGRO_SAMPLE* coin = NULL;
	ALLEGRO_SAMPLE* door = NULL;
	ALLEGRO_SAMPLE* bg = NULL;
	ALLEGRO_SAMPLE* descend = NULL;
	ALLEGRO_SAMPLE* button = NULL;
	ALLEGRO_SAMPLE* menu = NULL;
	ALLEGRO_SAMPLE* tree = NULL;
	ALLEGRO_SAMPLE* stone = NULL;
	ALLEGRO_SAMPLE* forest = NULL;
	ALLEGRO_SAMPLE* lava = NULL;
	ALLEGRO_SAMPLE* end = NULL;
	ALLEGRO_SAMPLE* heart = NULL;
	ALLEGRO_SAMPLE* walk = NULL;


	//program init
	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if (!display)										//test display object
		return -1;

	if (!al_install_mouse()) {
		al_show_native_message_box(display, "Error!", "Failed to initialize the mouse!\n.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return (-1);
	}

	//addon init
	al_install_keyboard();
	al_init_image_addon();
	al_init_primitives_addon();
	// font initialized
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_FONT* font3 = al_load_font("f.ttf", 35, 0);
	// check font created
	if (!font3) {
		al_show_native_message_box(NULL, "Error!", "Allegro has failed to initialize font.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}

	ALLEGRO_FONT* font2 = al_load_font("rb.ttf", 45, 0);
	// check font created
	if (!font2) {
		al_show_native_message_box(NULL, "Error!", "Allegro has failed to initialize font.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}
	ALLEGRO_FONT* font = al_load_font("rb.ttf", 25, 0);
	// check font created
	if (!font) {
		al_show_native_message_box(NULL, "Error!", "Allegro has failed to initialize font.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}
	// check audio init
	if (!al_install_audio()) {
		fprintf(stderr, "failed to initialize audio!\n");
		return -1;
	}
	// check acodec inits
	if (!al_init_acodec_addon()) {
		return -1;
	}

	// reserve space for all audio used
	if (!al_reserve_samples(samples)) {
		return -1;
	}

	//create sounds and check file exists

	// coin pickup sound effect
	coin = al_load_sample("coin.wav");

	if (!coin) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// door open sound effect
	door = al_load_sample("door.wav");

	if (!door) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// enter new map sound effect
	descend = al_load_sample("descend.wav");

	if (!descend) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// menu button sound effect
	button = al_load_sample("button.wav");

	if (!button) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// menu background music
	menu = al_load_sample("menut.wav");

	if (!menu) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// map3 door opening sound effect
	tree = al_load_sample("tree.wav");

	if (!tree) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// stone door sound effect
	stone = al_load_sample("stone.wav");

	if (!stone) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// map3 background music
	forest = al_load_sample("forest.wav");

	if (!forest) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// map4 background music
	lava = al_load_sample("lava.wav");

	if (!lava) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// post game background music
	end = al_load_sample("end.wav");

	if (!end) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// final pickup sound effect
	heart = al_load_sample("heart.wav");

	if (!heart) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// footsteps
	walk = al_load_sample("walk.wav");

	if (!walk) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	// status bar image
	ALLEGRO_BITMAP* status = NULL;
	ALLEGRO_BITMAP* coin_icon = NULL;
	ALLEGRO_BITMAP* clock_icon = NULL;
	ALLEGRO_BITMAP* logo = NULL;

	// load images and check files exist

	// status bar under game text
	// tracks map progress
	status = al_load_bitmap("bar0.png");
	if (!status) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}

	// coin icon on iu overlay
	coin_icon = al_load_bitmap("coin0.png");
	if (!coin_icon) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}

	// clock icon on iu overlay
	clock_icon = al_load_bitmap("timer.png");
	if (!clock_icon) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}

	// logo in menu screen
	logo = al_load_bitmap("logo.png");
	if (!logo) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}

	// menu music begins
	al_play_sample(menu, 0.4, 1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

	// create input queue
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);
	// register input method and timer
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);

	// current menu screen on
	// 0 is main, 1 is info
	int curScreen = 0;

	// main menu loop
	while (!done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		// every loop actions
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			// main screen posted
			if (curScreen == 0) {
				// begin button
				al_draw_tinted_bitmap(logo, al_map_rgba(38, 38, 38, 255), WIDTH / 6, 5, 0);
				al_draw_filled_rounded_rectangle( WIDTH / 4, HEIGHT / 4 + 50, WIDTH / 4 + 500, HEIGHT / 4 + 175, 5, 5, al_map_rgba(77, 99, 135, 255));
				al_draw_filled_rounded_rectangle(WIDTH / 4 - 15, HEIGHT / 4 + 35, WIDTH / 4 + 515, HEIGHT / 4 + 185, 5, 5, al_map_rgba(28, 68, 120, 200));
				al_draw_text(font2, al_map_rgba(199, 199, 199, 222), WIDTH / 4 + 30, HEIGHT / 4 + 85, ALLEGRO_ALIGN_LEFT, "BEGIN THE JOURNEY");
				// info button
				al_draw_filled_rounded_rectangle(WIDTH / 4, HEIGHT - 150, WIDTH / 4 + 200, HEIGHT - 25, 5, 5, al_map_rgba(128, 186, 164, 255));
				al_draw_filled_rounded_rectangle(WIDTH / 4 - 15, HEIGHT - 165, WIDTH / 4 + 215, HEIGHT - 10, 5, 5, al_map_rgba(65, 110, 93, 200));
				al_draw_text(font2, al_map_rgba(199, 199, 199, 222), WIDTH / 4 + 60, HEIGHT - 115, ALLEGRO_ALIGN_LEFT, "Info");
				// exit button
				al_draw_filled_rounded_rectangle(WIDTH / 4 + 310, HEIGHT - 150, WIDTH / 4 + 500, HEIGHT - 25, 5, 5, al_map_rgba(209, 50, 90, 255));
				al_draw_filled_rounded_rectangle(WIDTH / 4 + 295, HEIGHT - 165, WIDTH / 4 + 515, HEIGHT - 10, 5, 5, al_map_rgba(112, 33, 53, 200));
				al_draw_text(font2, al_map_rgba(199, 199, 199, 222), WIDTH / 4 + 365, HEIGHT - 115, ALLEGRO_ALIGN_LEFT, "Exit");
			}
			// info screen posted
			else {
				al_draw_text(font, al_map_rgba(199, 199, 199, 100), WIDTH / 2 , HEIGHT / 5, ALLEGRO_ALIGN_CENTER, "You have come to this Cave of Greed to test your fortune.");
				al_draw_text(font, al_map_rgba(199, 199, 199, 100), WIDTH / 2, HEIGHT / 5 * 1.5, ALLEGRO_ALIGN_CENTER, "It is said to weigh your pockets down with riches...");
				al_draw_text(font, al_map_rgba(199, 199, 199, 100), WIDTH / 2, HEIGHT / 5 * 2, ALLEGRO_ALIGN_CENTER, "...but make sure it does not drag you down too.");

				al_draw_text(font, al_map_rgba(199, 199, 199, 100), WIDTH / 2, HEIGHT / 5 * 3, ALLEGRO_ALIGN_CENTER, "Make it through each level of the cave before you are trapped inside.");
				al_draw_text(font, al_map_rgba(199, 199, 199, 100), WIDTH / 2, HEIGHT / 5 * 3.5, ALLEGRO_ALIGN_CENTER, "Use the arrow keys to move around.");
				al_draw_text(font, al_map_rgba(199, 199, 199, 100), WIDTH / 2, HEIGHT / 5 * 4.5, ALLEGRO_ALIGN_CENTER, "Click anywhere to return back to the main menu.");
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
		// mouse input detected
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			// pull coordinates from mouse location
			if (ev.mouse.button & 1)
			{
				int posX = ev.mouse.x;
				int posY = ev.mouse.y;
				if (curScreen == 0) {
					// begin button pressed
					if (posX >= WIDTH / 4 && posX <= WIDTH / 4 + 500 && posY >= HEIGHT / 4 + 50 && posY <= HEIGHT / 4 + 175) {
						done = true;
					}
					// info button pressed
					else if (posX >= WIDTH / 4 && posY >= HEIGHT - 150 && posX <= WIDTH / 4 + 200 && posY <= HEIGHT - 25) {
						al_play_sample(button, 0.50, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						curScreen = 1;
					}
					// exit button pressed
					else if (posX >= WIDTH / 4 + 310 && posY >= HEIGHT - 150 && posX <= WIDTH / 4 + 500 && posY <= HEIGHT - 25) {
						al_play_sample(button, 0.50, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						return -1;
					}
				}
				// return to main screen
				else {
					curScreen = 0;
				}

			}
		}
		// program exited
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			return -1;
		}
	}

	// menu music stopped
	al_stop_samples();
	al_play_sample(button, 0.50, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

	// background music started
	bg = al_load_sample("background.wav");

	if (!bg) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}

	al_play_sample(bg, 0.4, 1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

	// load sprite stats
	player.InitSprites(WIDTH, HEIGHT);

	// load map and offset of sprite
	int xOff = 0;
	int yOff = 0;
	if (MapLoad(map, 1))
		return -5;

	done = false;
	timer = al_create_timer(1.0 / 60);
	al_start_timer(timer);
	//draw the background tiles
	MapDrawBG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1);

	//draw foreground tiles
	MapDrawFG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1, 0);
	player.DrawSprites(480, 64);
	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));

	// main game loop
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		// every loop actions
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			render = true;
			// check user inputs
			if (keys[UP])
				player.UpdateSprites(WIDTH, HEIGHT, 3);
			if (keys[DOWN])
				player.UpdateSprites(WIDTH, HEIGHT, 4);
			if (keys[LEFT])
				player.UpdateSprites(WIDTH, HEIGHT, 0);
			if (keys[RIGHT])
				player.UpdateSprites(WIDTH, HEIGHT, 1);
			// if no valid movements, stand still
			if (keys[UP] == false && keys[DOWN] == false && keys[LEFT] == false && keys[RIGHT] == false)
				player.UpdateSprites(WIDTH, HEIGHT, 2);
			// if valid movement happened, play a footstep sound every 30 ticks (1/2 second)
			else
				if (stepDelay % 30 == 0)
					al_play_sample(walk, 0.15, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				stepDelay++;
			// check for end block reached
			if (player.CollisionEndBlock()) {
				cout << "And further we descend...\n";
				text = "And further we descend...";
				// map changes based on current map
				if (currentMap == 1) {
					cout << "The smell of moss and stale moisture surrounds you\n";
					text = "The smell of moss and stale moisture surrounds you";
					curSwitch = 0;
					if (MapLoad(map2, 1))
						return -5;
					// new status bar drawn
					status = al_load_bitmap("bar1.png");
					if (!status) {
						al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
							NULL, ALLEGRO_MESSAGEBOX_ERROR);
						al_destroy_display(display);
						return 0;
					}
					
					// reset timer
					totalTime += (al_get_timer_count(timer) / 60);
					timer = al_create_timer(1.0 / 60);
					al_start_timer(timer);
					//draw the background tiles
					MapDrawBG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1);

					//draw foreground tiles
					MapDrawFG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1, 0);
					player.setXY(55, 380);
					al_flip_display();
					currentMap = 2;
				}
				else if (currentMap == 2) {
					al_stop_samples();
					al_play_sample(forest, 0.5, 1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					cout << "Stone stairs become a muddy, dirt slope as you descend\n";
					text = "Stone stairs become a muddy, dirt slope as you descend";
					if (MapLoad(map3, 1))
						return -5;
					// new status bar drawn
					status = al_load_bitmap("bar2.png");
					if (!status) {
						al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
							NULL, ALLEGRO_MESSAGEBOX_ERROR);
						al_destroy_display(display);
						return 0;
					}
					// reset timer
					totalTime += (al_get_timer_count(timer) / 60);
					timer = al_create_timer(1.0 / 60);
					al_start_timer(timer);
					//draw the background tiles
					MapDrawBG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1);

					//draw foreground tiles
					MapDrawFG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1, 0);
					player.UpdateSprites(0, 64, 2);
					player.setXY(85, 64);
					al_flip_display();
					currentMap = 3;

					//reset variables
					curSwitch = 0;
					checkpoint = 0;
				}
				// if on third map, game ends
				else if(currentMap == 3){
					al_stop_samples();
					al_play_sample(lava, 0.5, 1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					cout << "Something bubbles behind you as you reach the new floor...\n";
					text = "Something bubbles behind you as you reach the new floor...";
					if (MapLoad(map4, 1))
						return -5;
					// new status bar drawn
					status = al_load_bitmap("bar3.png");
					if (!status) {
						al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
							NULL, ALLEGRO_MESSAGEBOX_ERROR);
						al_destroy_display(display);
						return 0;
					}
					// reset timer
					totalTime += (al_get_timer_count(timer) / 60);
					timer = al_create_timer(1.0 / 60);
					al_start_timer(timer);
					//draw the background tiles
					MapDrawBG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1);

					//draw foreground tiles
					MapDrawFG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1, 0);
					player.UpdateSprites(0, 64, 2);
					player.setXY(256, 72);
					al_flip_display();
					currentMap = 4;

					//reset variables
					curSwitch = 0;
					checkpoint = 0;
				}
				else if(currentMap == 4){
					al_stop_samples();
					cout << "You fall through the floor to a cave below\n";
					text = "You fall through the floor to a cave below";
					if (MapLoad(map5, 1))
						return -5;
					// new status bar drawn
					status = al_load_bitmap("bar3.png");
					if (!status) {
						al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
							NULL, ALLEGRO_MESSAGEBOX_ERROR);
						al_destroy_display(display);
						return 0;
					}
					// reset timer
					totalTime += (al_get_timer_count(timer) / 60);
					timer = al_create_timer(1.0 / 60);
					al_start_timer(timer);
					//draw the background tiles
					MapDrawBG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1);

					//draw foreground tiles
					MapDrawFG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1, 0);
					player.UpdateSprites(0, 64, 2);
					player.setXY(1200, 256);
					al_flip_display();
					currentMap = 5;

					//reset variables
					curSwitch = 0;
					checkpoint = 0;

					// ending depending on player's choices
					if (money == 0) {
						MapSetBlock(95, 7, 480);
					}
					else if (money == 120) {
						MapSetBlock(95, 7, 482);
					}
					else {
						MapSetBlock(95, 7, 481);
					}
				}
				// all maps finished, end game begins
				else {
					currentMap = 6;
					done = true;
				}
				// audio cue for new level
				al_play_sample(descend, 0.75, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}

			// map specific actions
			if (currentMap == 1) {
				// pulls value of block the player currently on
				int blockValue = player.CollisionKeyBlock();
				// coin block
				if (blockValue == 3) {
					cout << "Coin obtained" << endl;
					text = "Coin obtained";
					al_play_sample(coin, 1.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					money++;
					int blockX = (player.getX() + player.getWidth() / 2) / mapblockwidth;
					int blockY = (player.getY() + player.getHeight() + 5) / mapblockheight;
					MapSetBlock(blockX, blockY, 214);
				}
				// switch block
				if (blockValue == 1) {
					al_play_sample(door, 1.65, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					// switches do different actions based on which pressed
					// since they must be done in order they can just be tracked with this int
					if (curSwitch == 0) {
						cout << "The front gates rumble in the distance!" << endl;
						text = "The front gates rumble in the distance!";
						// entrance gate blocks become blocks with no collision
						MapSetBlock(17, 8, 214);
						MapSetBlock(17, 9, 214);
						MapSetBlock(18, 8, 214);
						MapSetBlock(18, 9, 214);
						// switch becomes a deactivated switch to prevent multiple activations
						MapSetBlock(2, 9, 480);
						// increment last switch activated
						curSwitch++;
					}
					// switch that opens exit to player
					else if (curSwitch == 1) {
						cout << "The entrance to the stairway has cleared!" << endl;
						text = "The entrance to the stairway has cleared!";
						// clear wall
						for (int i = 8; i < 10; i++) {
							for (int j = 40; j < 48; j++) {
								MapSetBlock(j, i, 214);
							}
						}
						// deactivate switch
						MapSetBlock(21, 2, 480);
						curSwitch++;
					}
				}
			}
			// map specific actions
			else if (currentMap == 2) {
				int blockValue = player.CollisionKeyBlock();
				// switch block
				if (blockValue == 1) {
					al_play_sample(door, 1.65, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					if (curSwitch == 0) {
						cout << "The sound of running water fills the chamber below..." << endl;
						text = "The sound of running water fills the chamber below...";
						for (int i = 10; i < 12; i++) {
							for (int j = 12; j < 15; j++) {
								MapSetBlock(i, j, 46);
							}
						}
						MapSetBlock(8, 4, 480);
						curSwitch++;
					}
				}
				// coin block
				else if (blockValue == 3) {
					cout << "Greed increased" << endl;
					text = "Greed increased";
					al_play_sample(coin, 1.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					money++;
					int blockX = (player.getX() + player.getWidth() / 2) / mapblockwidth;
					int blockY = (player.getY() + player.getHeight() + 5) / mapblockheight;
					MapSetBlock(blockX, blockY, 46);
				}

				// water block
				else if (blockValue == 2) {
					// water sends you back to last checkpoint if one has been reached
					// start of level
					if (checkpoint == 0) {
						player.setXY(55, 380);
						cout << "You sink into the water and back to the entrance...\n" << endl;
						text = "You sink into the water and back to the entrance...";
					}
					// before large room water puzzle
					else if (checkpoint == 1) {
						player.setXY(750, 380);
						cout << "You clutch onto nearby rocks after slipping down...\n" << endl;
						text = "You clutch onto nearby rocks after slipping down...";
					}
					// before first hallway water puzzle
					else if (checkpoint == 2) {
						player.setXY(1800, 80);
						cout << "You slide back down to the beginning of the corridor...\n" << endl;
						text = "You slide back down to the beginning of the corridor...";
					}
					// before second hallway water puzzle
					else if (checkpoint == 3) {
						player.setXY(2000, 420);
						cout << "The flow of water sweeps your feet from below you\n" << endl;
						text = "The flow of water sweeps your feet from below you";
					}
				}
				// checkpoints set here
				// when reached, they become the player's new respawn point
				// before large room water puzzle
				else if (blockValue == 4) {
					checkpoint = 1;
				}
				// before first hallway puzzle
				else if (blockValue == 5) {
					checkpoint = 2;
				}
				// before second hallway puzzle
				else if (blockValue == 6) {
					checkpoint = 3;
				}
			}
			// specific triggers for map 3
			else if (currentMap == 3) {
				int blockValue = player.CollisionKeyBlock();
				// switch blocks - in this map these are dead trees
				// a new technical challenge is that these switches can be pressed in any order
				if (blockValue == 1) {
					al_play_sample(tree, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					cout << "The dead tree falls and sinks into the muddy ground" << endl;
					text = "The dead tree falls and sinks into the muddy ground";
					// no matter what switch is pressed, the next row of trees are cleared at the exit
					if (curSwitch == 0) {
						for (int i = 1; i < 4; i++) {
								MapSetBlock(13, i, 480);
						}
						
					}
					// for loops used to reduce size
					else if (curSwitch == 1) {
						for (int i = 1; i < 4; i++) {
							MapSetBlock(12, i, 480);
						}
					}
					// trees replaced with stumps to hint at player proper solution
					else if (curSwitch == 2) {
						for (int i = 1; i < 4; i++) {
							MapSetBlock(11, i, 480);
						}
					}
					// default text on screen replaced to let player know the entrance is open
					else if (curSwitch == 3) {
						cout << "A warm gust of air fills the chamber as the tree falls..." << endl;
						text = "A warm gust of air fills the chamber as the tree falls...";
						for (int i = 1; i < 4; i++) {
							MapSetBlock(10, i, 480);
						}
					}
					// x ranges used to find which tree was pressed
					// selected tree is replaced with stump to prevent multiple presses
					if (player.getX() < 350) {
						MapSetBlock(2, 14, 480);
					}
					else if (player.getX() < 1200) {
						MapSetBlock(30, 16, 480);
					}
					else if (player.getX() < 2500) {
						MapSetBlock(70, 8, 480);
					}
					else {
						MapSetBlock(92, 3, 480);
					}
					// increment switch to make sure next line of trees is removed
					curSwitch++;
				}
				// coin block
				else if (blockValue == 3) {
					cout << "Loot stolen" << endl;
					text = "Loot stolen";
					al_play_sample(coin, 1.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					money++;
					// block replaced with default tile texture
					int blockX = (player.getX() + player.getWidth() / 2) / mapblockwidth;
					int blockY = (player.getY() + player.getHeight() + 5) / mapblockheight;
					MapSetBlock(blockX, blockY, 239);
				}
			}
			// map 4 specific triggers
			else if (currentMap == 4) {
			// unique mechanic to this level is lava moving from left to right every 50 ticks (5/6th of a second)
			// lavaTime increments every tick, lava progresses at 50 and resets lavaTime to 0
			lavaTime++;
			if (lavaTime >= 50) {
				lavaTime = 0;
				// current index lava is on
				currentLava++;
				// lava blocks placed on next available column
				for (int i = 0; i < 17; i++) {
					MapSetBlock(currentLava, i, 4);
				}
			}
			// pull current player's block value
			int blockValue = player.CollisionKeyBlock();
			// switch blocks
			// only one switch on this level
			if (blockValue == 1) {
				al_play_sample(stone, 0.40, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				cout << "You can barely hear the wall shift above the sound of lava" << endl;
				text = "You can barely hear the wall shift above the sound of lava";
				if (curSwitch == 0) {
					// remove wall blocks behind player
					for (int i = 67; i < 69; i++) {
						for (int j = 11; j < 13; j++) {
							MapSetBlock(i, j, 99);
						}
						// replace switch with deactivated switch
						MapSetBlock(91, 13, 481);
					}
				}
				curSwitch++;
			}
			// coin block
			else if (blockValue == 3) {
				cout << "..." << endl;
				text = "...";
				al_play_sample(coin, 1.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				money++;
				// replace money block with default tile texture
				int blockX = (player.getX() + player.getWidth() / 2) / mapblockwidth;
				int blockY = (player.getY() + player.getHeight() + 5) / mapblockheight;
				MapSetBlock(blockX, blockY, 99);
			}
			// lava block touched
			else if (blockValue == 4) {
				// game immediately ends on contact
				currentLava = -1;
				done = true;
				totalTime = -1;
			}
		}

			render = true;
			// update animated blocks
			MapUpdateAnims();

		}
		// window closed, end game
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			closed = true;
			totalTime = -1;
			done = true;
		}
		// key held down
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			// end game as if time had run out
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				closed = true;
				totalTime = -1;
				break;
			// movement keys selected; multiple can be active at once
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			// space not used but kept in code for possible expansion
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;

			}
		}
		// key unpressed
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			// game ended even on Escape key unpress
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				closed = true;
				totalTime = -1;
				break;
			// movements ended
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			// space not used but kept in for future implementation
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}
		// no events left and display must be rendered
		if (render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			//update the map scroll position
			xOff = player.getX() + player.getWidth() - WIDTH / 2;
			yOff = player.getY() + player.getHeight() - HEIGHT / 2;

			//avoid moving beyond the map edge
			if (xOff < 0) xOff = 0;

			if (xOff > (mapwidth * mapblockwidth - WIDTH))
				xOff = mapwidth * mapblockwidth - WIDTH;
			if (yOff < 0)
				yOff = 0;
			if (yOff > (mapheight * mapblockheight - HEIGHT))
				yOff = mapheight * mapblockheight - HEIGHT;

			//draw the background tiles
			MapDrawBG(xOff, yOff, 0, 0, WIDTH, HEIGHT);

			//draw foreground tiles
			MapDrawFG(xOff, yOff, 0, 0, WIDTH, HEIGHT, 0);

			// UI features
			// these are all removed except money during final map for  A R T I S T I C  R E A S O N S
			if (currentMap < 5) {
				// timer elements
				al_draw_rotated_bitmap(clock_icon, 16, 16, 20, HEIGHT - 25, 3.14, 0);
				al_draw_textf(font2, al_map_rgba(186, 213, 255, 150), 40, HEIGHT - 50, ALLEGRO_ALIGN_LEFT, "%d", 120 - ((al_get_timer_count(timer) / 60)));
				// dialogue elements
				al_draw_textf(font, al_map_rgba(173, 173, 173, 180), WIDTH - 10, HEIGHT - 80, ALLEGRO_ALIGN_RIGHT, "%s", text.c_str());
				al_draw_scaled_bitmap(status, 0, 0, 150, 24, WIDTH - 325, HEIGHT - 50, 300, 48, 0);
			}
			// coin elements
			al_draw_rotated_bitmap(coin_icon, 16, 16, 135, HEIGHT - 25, 3.14, 0);
			al_draw_textf(font2, al_map_rgba(214, 214, 109, 150), 150, HEIGHT - 50, ALLEGRO_ALIGN_LEFT, "%d", money);
			
			// player drawn
			player.DrawSprites(xOff, yOff);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));

			// if during a map 120 seconds is passed, game will end
			if (120 - ((al_get_timer_count(timer) / 60)) == 0 && currentMap != 5) {
				done = true;
				// special ending for losing on first level
				if (currentMap != 1) {
					totalTime = -1;
				}
				else {
					totalTime = 120;
				}
			}
		}
	}

	// post game stat screen

	// to delay, we use allegro's timer to grab the current time
	// when the difference between that time and the current time is x seconds, the code continues
	int64_t time = al_get_timer_count(timer);
	// all music/effects ended
	al_stop_samples();
	// end music begins
	al_play_sample(end, 0.25, 1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	// player has won game
	if (currentMap == 6) {
		al_play_sample(heart, 1.00, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		// 3.3 second delay
		time = al_get_timer_count(timer);
		while (time + 200 > al_get_timer_count(timer)) {}
		al_draw_text(font3, al_map_rgba(104, 119, 168, 150), WIDTH / 2, HEIGHT / 7, ALLEGRO_ALIGN_CENTER, "The treasure of the cave is now before you...");
		al_flip_display();

		// 3.3 second delay
		time = al_get_timer_count(timer);
		while (time + 200 > al_get_timer_count(timer)) {}

		// ending 1 - player collected no coins
		if (money == 0) {
			al_draw_text(font3, al_map_rgb(111, 179, 201), WIDTH / 2, HEIGHT / 7 * 2, ALLEGRO_ALIGN_CENTER, "It shines with a glistening newness like it had never been touched before.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}
			
			al_draw_text(font3, al_map_rgb(111, 179, 201), WIDTH / 2, HEIGHT / 7 * 3, ALLEGRO_ALIGN_CENTER, "In a moment, you blink and find yourself back outside the entrance.");
			al_flip_display();
			
			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(111, 179, 201), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "Still holding the heart, it hums with some ancient power.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(111, 179, 201), WIDTH / 2, HEIGHT / 7 * 5, ALLEGRO_ALIGN_CENTER, "You can only guess what the cave has granted you.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(111, 179, 201), WIDTH / 2, HEIGHT / 7 * 6, ALLEGRO_ALIGN_CENTER, "ASCENDANCE ENDING");
			al_flip_display();

		}
		// ending 2 - player collected all coins
		else if (money == 120) {
			al_draw_text(font3, al_map_rgb(171, 118, 148), WIDTH / 2, HEIGHT / 7 * 2, ALLEGRO_ALIGN_CENTER, "When you attempt to touch it, it shatters into pieces on the ground.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(171, 118, 148), WIDTH / 2, HEIGHT / 7 * 3, ALLEGRO_ALIGN_CENTER, "With it, you feel your loaded pockets thin, ashes replacing the shiny metal.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(171, 118, 148), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "The cave grows dark, leaving you with nowhere to go.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(171, 118, 148), WIDTH / 2, HEIGHT / 7 * 6, ALLEGRO_ALIGN_CENTER, "GREED ENDING");
			al_flip_display();
		}
		// ending 3 - some amount of coins below 120 is collected
		else {
			al_draw_text(font3, al_map_rgb(152, 121, 199), WIDTH / 2, HEIGHT / 7 * 2, ALLEGRO_ALIGN_CENTER, "When you take hold of it, a bright flash stings your eyes.");
			al_flip_display();
			
			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(152, 121, 199), WIDTH / 2, HEIGHT / 7 * 3, ALLEGRO_ALIGN_CENTER, "Your eyes adjust and you find yourself back at the entrance again.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(152, 121, 199), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "The heart is gone, along with your loot.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(152, 121, 199), WIDTH / 2, HEIGHT / 7 * 5, ALLEGRO_ALIGN_CENTER, "The door is still open, however, for you to try again.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(152, 121, 199), WIDTH / 2, HEIGHT / 7 * 6, ALLEGRO_ALIGN_CENTER, "LOOP ENDING");
			al_flip_display();
		}
		
	}
	// game ends prior to finishing final level
	else {

	// 3.3 second delay
	time = al_get_timer_count(timer);
	while (time + 200 > al_get_timer_count(timer)) {}
	al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7, ALLEGRO_ALIGN_CENTER, "Your journey has ended...");
	al_flip_display();

	// 3.3 second delay
	time = al_get_timer_count(timer);
	while (time + 200 > al_get_timer_count(timer)) {}

		// ending 4 - player lost on map4
		if (currentMap == 4) {
			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 2, ALLEGRO_ALIGN_CENTER, "The lava flow is too quick for you and you meet your demise.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 3, ALLEGRO_ALIGN_CENTER, "The treasure of the cave is lost forever.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 6, ALLEGRO_ALIGN_CENTER, "BAD ENDING");
			al_flip_display();
		}
		// ending 5 - player lost to map3
		else if (currentMap == 3) {
			al_draw_text(font3, al_map_rgb(131, 166, 134), WIDTH / 2, HEIGHT / 7 * 2, ALLEGRO_ALIGN_CENTER, "You find the forest too peaceful to ever leave.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(131, 166, 134), WIDTH / 2, HEIGHT / 7 * 3, ALLEGRO_ALIGN_CENTER, "The cave becomes your new home, tending it and living off it.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(131, 166, 134), WIDTH / 2, HEIGHT / 7 * 6, ALLEGRO_ALIGN_CENTER, "SERENE ENDING");
			al_flip_display();
		}
		// ending 6 - player lost to map2
		else if (currentMap == 2) {
			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 2, ALLEGRO_ALIGN_CENTER, "The cave floods with water and you are swept away.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 3, ALLEGRO_ALIGN_CENTER, "Your loot is scattered far and wide for others to find.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 6, ALLEGRO_ALIGN_CENTER, "BAD ENDING");
			al_flip_display();
		}
		// ending 7 - player lost to map1
		else if (currentMap == 1) {
			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 2, ALLEGRO_ALIGN_CENTER, "Smartly, you decide against entering the cave altogether.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 3, ALLEGRO_ALIGN_CENTER, "Debts can pay themselves other ways, this looks dangerous.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 6, ALLEGRO_ALIGN_CENTER, "REASONABLE ENDING");
			al_flip_display();
		}
		// ending 8 - player force quits on map5
		else if (currentMap == 5) {
			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 2, ALLEGRO_ALIGN_CENTER, "You hear the developer speak directly to you as you quit at the end.");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 3, ALLEGRO_ALIGN_CENTER, "\"Honestly I'm impressed that you lost a level with no hazards\".");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "\"Literally just walk to the right side and touch the heart\".");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 5, ALLEGRO_ALIGN_CENTER, "\"Were you upset with your ending? You earned it\".");
			al_flip_display();

			// 3.3 second delay
			time = al_get_timer_count(timer);
			while (time + 200 > al_get_timer_count(timer)) {}

			al_draw_text(font3, al_map_rgb(145, 122, 133), WIDTH / 2, HEIGHT / 7 * 6, ALLEGRO_ALIGN_CENTER, "META ENDING");
			al_flip_display();
		}
	}

	// 6.6 second delay
	time = al_get_timer_count(timer);
	while (time + 400 > al_get_timer_count(timer)) {}

	// clear ending screen
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	// RESULTS SCREEN

	al_draw_text(font3, al_map_rgb(168, 118, 204), WIDTH / 2, HEIGHT / 7 * 1, ALLEGRO_ALIGN_CENTER, "RESULTS");
	al_flip_display();

	// 0.5 second delay
	time = al_get_timer_count(timer);
	while (time + 30 > al_get_timer_count(timer)) {}

	// print out money earned
	al_draw_textf(font3, al_map_rgb(203, 219, 96), WIDTH / 2, HEIGHT / 7 * 2, ALLEGRO_ALIGN_CENTER, "YOUR POCKETS ARE %d COINS HEAVIER.", money);
	al_flip_display();

	// 0.5 second delay
	time = al_get_timer_count(timer);
	while (time + 30 > al_get_timer_count(timer)) {}

	// due to ending logic, time should not show up as -1
	if (currentMap == 1) {
		totalTime = 0;
	}
	// time spent in game
	// if bad ending (except map1) was had
	if ((totalTime == -1 || closed == true) && currentMap != 1) {
		al_draw_text(font3, al_map_rgb(135, 144, 156), WIDTH / 2, HEIGHT / 7 * 3, ALLEGRO_ALIGN_CENTER, "TIME SPENT IN THE DUNGEON: An Eternity.");
	}
	// if game finished
	else {
		al_draw_textf(font3, al_map_rgb(135, 179, 237), WIDTH / 2, HEIGHT / 7 * 3, ALLEGRO_ALIGN_CENTER, "TIME SPENT IN THE DUNGEON: %d seconds.", totalTime);
	}
	al_flip_display();

	// 0.5 second delay
	time = al_get_timer_count(timer);
	while (time + 30 > al_get_timer_count(timer)) {}

	// ranking based on time
	// player left at beginning
	if (currentMap == 1) {
		al_draw_text(font3, al_map_rgb(145, 145, 145), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "RANK EARNED: A Secret Cat.");
	}
	// player left at very end
	else if (currentMap == 5) {
		al_draw_text(font3, al_map_rgb(145, 145, 145), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "RANK EARNED: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	}
	// player died before finishing
	else if (totalTime == -1 || closed == true) {
		al_draw_text(font3, al_map_rgb(145, 145, 145), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "RANK EARNED: A Doornail");
	}
	// player spend almost max time on every level
	else if (totalTime >= 430) {
		al_draw_text(font3, al_map_rgb(218, 230, 204), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "RANK EARNED: Shockingly Impressive Sloth");
	}
	// player spend least time possible on all levels (fastest time finished was 170)
	else if (totalTime <= 175) {
		al_draw_text(font3, al_map_rgb(235, 235, 235), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "RANK EARNED: Fast-Food Fast Speed Hedgehog");
	}
	// times between max and min
	else if (totalTime <= 200) {
		al_draw_text(font3, al_map_rgb(217, 217, 217), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "RANK EARNED: Angry Bear");
	}
	else if (totalTime <= 250) {
		al_draw_text(font3, al_map_rgb(196, 196, 196), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "RANK EARNED: Beaver with Downhill Momentum");
	}
	else if (totalTime <= 300) {
		al_draw_text(font3, al_map_rgb(181, 181, 181), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "RANK EARNED: A Red Panda. Adorably Inefficient");
	}
	else if (totalTime <= 350) {
		al_draw_text(font3, al_map_rgb(171, 171, 171), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "RANK EARNED: A Curious Koala. Smelling the Roses");
	}
	else if (totalTime <= 400) {
		al_draw_text(font3, al_map_rgb(156, 156, 156), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "RANK EARNED: A Turtle. You Win...Eventually");
	}
	// should never trigger but let's leave this here
	else {
		al_draw_text(font3, al_map_rgb(255, 0, 225), WIDTH / 2, HEIGHT / 7 * 4, ALLEGRO_ALIGN_CENTER, "RANK EARNED: A Confused Developer");
	}

	al_flip_display();

	// 0.5 second delay
	time = al_get_timer_count(timer);
	while (time + 30 > al_get_timer_count(timer)) {}

	al_draw_text(font3, al_map_rgb(111, 179, 201), WIDTH / 2, HEIGHT / 7 * 6, ALLEGRO_ALIGN_CENTER, "Click to end the game...until next time.");

	al_flip_display();

	// final game loop
	// waits for mouse click and then quits
	done = false;
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			// end game on any click
			if (ev.mouse.button & 1)
			{
				done = true;
			}
		}
	}
	// delete map memory
	MapFreeMem();
	// delete event queue
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);	//destroy our display object
	// delete all audio samples
	al_destroy_sample(coin);
	al_destroy_sample(door);
	al_destroy_sample(bg);
	al_destroy_sample(descend);
	al_destroy_sample(button);
	al_destroy_sample(tree);
	al_destroy_sample(menu);
	al_destroy_sample(stone);
	al_destroy_sample(forest);
	al_destroy_sample(lava);
	al_destroy_sample(end);
	al_destroy_sample(heart);
	al_destroy_sample(walk);
	return 0;
}

#endif