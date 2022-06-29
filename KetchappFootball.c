//Include directives
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <time.h> 
#include <allegro5/allegro_image.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/bitmap.h>


//Definitions
#define TOTAL_AIM 2
#define AIM  1
#define GOALKEEPER 1
#define BACKGROUND_FILE  "images/ground.png" 
#define SKY    "images/sky.png" 
#define SOCCER "images/soccer.png"
#define TAIM "images/aim.png"
#define BALL "images/ball.png" 
#define GAMEOVER "images/gameover.png"
#define MENU "images/menu.png"
#define GOALKEEP "images/goalkeeper.png"


//prototypes
void initAim(struct item aim[]);
void moveGoal(struct item goal[]);
void initGoal(struct item goal[]);
void placeAim(struct item aim[]);
void placeGoal(struct item goal[]);
int controlCrash(struct item aim, int x, int y);
int controlCrashGoal(struct item goal, int x, int y);
int practicegame();
int timechallengegame();
int freekickgame();
int main();


int width = 800;     //X length of the display
int height = 750;    //Y length of the display

enum KEYS { RIGHT, LEFT, UP, DOWN, ENTER };   //Keys alternate between up,down,enter, right and left

struct item {
	int x;
	int y;
	bool active;
};




void initAim(struct item aim[]) { // Baþlangýçta tüm hedeflerin active deðiþkenini false yapmak için kullanýlan fonk.

	for (int i = 0; i < TOTAL_AIM; i++) {

		aim[i].active = false;
	}

}

int goalDirection = 1;

void moveGoal(struct item goal[]) {


	if (goalDirection == 1) {
		goal->x -= 2;

	}
	else if (goalDirection == 2) {
		goal->x += 2;

	}

	if (goal->x < 200) {

		goalDirection = 2;

	}
	else if (goal->x > 550)
	{
		goalDirection = 1;
	}



}



void initGoal(struct item goal[]) {
	for (int i = 0; i < GOALKEEPER; i++) {
		goal[i].active = false;
	}
}

void placeAim(struct item aim[]) {  // Konumlandýrýlmamýþ hedeflere random konum atayan fonksiyon

	int i;
	for (i = 0; i < TOTAL_AIM; i++) {
		if (aim[i].active == false) {           //If the aim is not drawn
			aim[i].x = 250 + rand() % 300;      //Randomly assigned x coordinate
			aim[i].y = 25 + rand() % 100;       //Randomly assigned y coordinate
			aim[i].active = true;               //In order not to reposition
		}
	}
}

void placeGoal(struct item goal[]) {
	int i;
	for (i = 0; i < GOALKEEPER; i++) {
		if (goal[i].active == false) {         //If the goalkeeper is not drawn
			goal[i].x = 250 + rand() % 250;    //Randomly assigned x coordinate
			goal[i].y = 50 + rand() % 90;      //Randomly assigned y coordinate
			goal[i].active = true;             //In order not to reposition 
		}
	}
}




int controlCrash(struct item aim, int x, int y) { //Function that controls collision for aims
	if (abs(aim.x - x) <= 50 && abs(aim.y - y) <= 20) {
		return 1;
	}
	return 0;
}

int controlCrashGoal(struct item goal, int x, int y) {  //Function that controls collision for goalkeeper
	if (abs(goal.x - x) <= 50 && abs(goal.y - y) <= 20) {
		return 1;
	}
	return 0;
}




int lives = 3; //Number of lives in the game
int timerGame = 20; //Time in the game




/*PRACTICE MODE */

int practicegame() {

	bool KEY[3] = { false,false,false };
	/*
	If you press any key, that key will be true

	KEY[0]=KEY[RIGHT]
	KEY[1]=KEY[LEFT]
	KEY[2]=KEY[UP]

	*/

	srand(time(NULL));
	al_init();                            //Initialize allegro functions
	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();



	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_TIMER* timer2 = NULL;
	ALLEGRO_BITMAP* background = NULL;
	ALLEGRO_BITMAP* sky = NULL;
	ALLEGRO_BITMAP* soccer = NULL;
	ALLEGRO_BITMAP* taim = NULL;
	ALLEGRO_BITMAP* ball = NULL;
	ALLEGRO_BITMAP* gameover = NULL;




	struct item aim[AIM];       // aims
	initAim(aim);


	ALLEGRO_FONT* font = al_load_font("font/pes.ttf", 20, 0);   //Font size is 20 
	ALLEGRO_FONT* font54 = al_load_font("font/pes.ttf", 54, 0); //Font size is 54
	al_reserve_samples(3);   //By number of sounds
	ALLEGRO_SAMPLE* sample = al_load_sample("sounds/yeehaw.wav");
	ALLEGRO_SAMPLE* samplekick = al_load_sample("sounds/kick.mp3");
	ALLEGRO_SAMPLE* whistle = al_load_sample("sounds/whistle.mp3");




	int xpos = 400, ypos = 670;
	int kickx = xpos, kicky = ypos;
	int score = 0;
	int FPS = 60;

	bool kicking = false; // Is your ball kicking right now?
	bool kicked = false;  // Has the goal been shot?
	bool draw = true;





	display = al_create_display(width, height);  //Display size
	queue = al_create_event_queue();  // for movement



	timer2 = al_create_timer(1); // timer of game
	timer = al_create_timer(1.0 / FPS); //speed
	ALLEGRO_EVENT_QUEUE* queue2 = al_create_event_queue(); //for timer of FPS


	float r = 20;


	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_timer_event_source(timer2));

	//BITMAPS
	background = al_load_bitmap(BACKGROUND_FILE);
	soccer = al_load_bitmap(SOCCER);
	sky = al_load_bitmap(SKY);
	taim = al_load_bitmap(TAIM);
	ball = al_load_bitmap(BALL);
	gameover = al_load_bitmap(GAMEOVER);

	al_start_timer(timer);
	al_start_timer(timer2);

	while (1) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);
		placeAim(aim);
		if (lives == 0 || timerGame == 0) {// Stop game

			//al_clear_to_color(al_map_rgb(0, 127, 0));
			al_draw_bitmap(background, 0, 0, 0);
			al_draw_bitmap(gameover, 200, 200, 0);
			al_draw_textf(font, al_map_rgb(255, 255, 255), 80, 150, 0, "YOUR SCORE  : %d ", score);

			al_play_sample(whistle, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			al_flip_display(display);
			break;

		}


		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			lives = 0; timerGame = 0;
		}


		al_flip_display();

		if (ev.type == ALLEGRO_EVENT_TIMER) {

			draw = true;
			if (ev.timer.source == timer2) {

				timerGame++;

			}


		}



		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {               // Event key is pressed
			if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				KEY[RIGHT] = true; // Right key pressed

			}

			else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				KEY[LEFT] = true; // Left key pressed

			}


			if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {

				kicking = true; //Start kick
				al_play_sample(samplekick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			}

			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {

				lives = 0; timerGame = 0;

			}



		}



		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {

				KEY[RIGHT] = false;

			}
			else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				KEY[LEFT] = false;

			}



		}


		// Left and right movement( does not move while kicking)
		if (KEY[LEFT] == true) {

			xpos -= 8;
			if (xpos < 50)
				xpos = 50;
		}
		if (KEY[RIGHT] == true) {
			xpos += 8;
			if (xpos > 750)
				xpos = 750;

		}


		if (kicking == true) { //If kicking
			kicky -= 10;

			r -= 0.15;
			if (kicky < 0 || kicked == true) {

				if (kicked == true) {
					al_stop_samples();
					score++;
					al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

				}
				else if (kicky < 0)
					lives--;

				kicking = false;
				kicked = false;
				kicky = ypos - 25;
				r = 25;
				//al_flush_event_queue(queue);


			}



		}

		if (!kicking) {// To prevent the movement of the ball to the right and left at the time of the kick

			kickx = xpos - 25;
			kicky = ypos - 25;

		}
		if (draw == true) { // Drawing by FPS
			al_clear_to_color(al_map_rgb(193, 255, 255));
			al_draw_bitmap(sky, 0, 0, 0);                 //background
			al_draw_line(0, 100, 800, 100, al_map_rgb(139, 69, 19), 40);
			al_draw_bitmap(background, 0, 120, 0);

			/*         al_draw_line(227, 25, width - 248, 25, al_map_rgb(255, 0, 255), 5);//top
						al_draw_line(230, 24, 230, 180, al_map_rgb(255, 0, 255), 5);//left
						al_draw_line(550, 24, 550, 180, al_map_rgb(255, 0, 255), 5);//right       */
			al_draw_bitmap(soccer, 225, 25, 0);

			int i = 0;

			while (i < AIM) {	//Draw aim(s)

				if (aim[i].active == true) {
					//al_draw_filled_circle(aim[i].x, aim[i].y, 15, al_map_rgb(0, 78, 91));
					al_draw_bitmap(taim, aim[i].x, aim[i].y, 15);
				}
				i++;
			}


			al_draw_filled_circle(xpos, ypos, 25, al_map_rgb(255, 255, 0)); //Position of the ball
			al_draw_line(xpos, ypos - 20, xpos, ypos - 51, al_map_rgb(255, 255, 0), 5);
			al_draw_filled_triangle(xpos, ypos - 71, xpos - 15, ypos - 51, xpos + 15, ypos - 51, al_map_rgb(255, 255, 0));



			//al_draw_filled_circle(kickx, kicky, r, al_map_rgb(255, 127, 0)); //Position of the thrown ball

			al_draw_bitmap(ball, kickx, kicky, r);




			al_draw_line(0, 710, width, 710, al_map_rgb(255, 0, 255), 5);
			al_draw_line(2, 708, 2, 750, al_map_rgb(255, 0, 255), 5);
			al_draw_line(400, 708, 400, 750, al_map_rgb(255, 0, 255), 5);
			al_draw_line(798, 708, 798, 750, al_map_rgb(255, 0, 255), 5);
			al_draw_line(0, 748, width, 748, al_map_rgb(255, 0, 255), 5);
			al_draw_textf(font, al_map_rgb(242, 239, 48), 100, 720, ALLEGRO_ALIGN_LEFT, "SCORE: %d  ", score);
			al_draw_textf(font, al_map_rgb(242, 239, 48), 550, 720, 0, "LIVES : %d ", lives);

			al_flip_display();

			draw = false;

		}

		for (int i = 0; i < AIM; i++) {

			if (controlCrash(aim[i], kickx, kicky) == 1) { //Collision 
				aim[i].active = false; //Reposition aim if collision occurred
				kicked = 1;

				r = 20;

			}

		}


	}

	al_rest(2);
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_sample(sample);
	al_destroy_sample(samplekick);
	al_destroy_sample(whistle);

	return 0;
}

/*TIME CHALLENGE MODE   */
int timechallengegame() {


	bool KEY[3] = { false,false,false };

	srand(time(NULL));
	al_init();                       //Initialize allegro functions
	al_install_keyboard();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();



	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_TIMER* timer2 = NULL;
	ALLEGRO_BITMAP* background = NULL;
	ALLEGRO_BITMAP* sky = NULL;
	ALLEGRO_BITMAP* soccer = NULL;
	ALLEGRO_BITMAP* taim = NULL;
	ALLEGRO_BITMAP* ball = NULL;
	ALLEGRO_BITMAP* gameover = NULL;
	ALLEGRO_BITMAP* goalkeep = NULL;




	struct item aim[TOTAL_AIM];  //Aims
	struct item goal[GOALKEEPER];//Goalkeeper
	initAim(aim);
	initGoal(goal);






	ALLEGRO_FONT* font = al_load_font("font/pes.ttf", 20, 0); //Font size is 20
	ALLEGRO_FONT* font54 = al_load_font("font/pes.ttf", 54, 0);////Font size is 54
	al_reserve_samples(3); //By number of sounds
	ALLEGRO_SAMPLE* sample = al_load_sample("sounds/yeehaw.wav");
	ALLEGRO_SAMPLE* samplekick = al_load_sample("sounds/kick.mp3");
	ALLEGRO_SAMPLE* whistle = al_load_sample("sounds/whistle.mp3");


	int xpos = 400, ypos = 670;
	int kickx = xpos, kicky = ypos;
	int score = 0;
	int FPS = 60;

	bool kicking = false; // Is your ball kicking right now?
	bool kicked = false;  // Has the goal been shot? 
	bool draw = true;


	display = al_create_display(width, height);  //Display size
	queue = al_create_event_queue();  // for movement


	timer2 = al_create_timer(1); // timer of game
	timer = al_create_timer(1.0 / FPS); //speed
	ALLEGRO_EVENT_QUEUE* queue2 = al_create_event_queue(); //for timer of FPS


	float r = 20;


	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_timer_event_source(timer2));

	//BITMAPS
	background = al_load_bitmap(BACKGROUND_FILE);
	soccer = al_load_bitmap(SOCCER);
	sky = al_load_bitmap(SKY);
	taim = al_load_bitmap(TAIM);
	ball = al_load_bitmap(BALL);
	gameover = al_load_bitmap(GAMEOVER);
	goalkeep = al_load_bitmap(GOALKEEP);

	al_start_timer(timer);
	al_start_timer(timer2);

	while (1) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);
		placeAim(aim);
		placeGoal(goal);
		if (lives == 0 || timerGame == 0) { //Stop game

			//al_clear_to_color(al_map_rgb(0, 127, 0));
			al_draw_bitmap(background, 0, 0, 0);
			al_draw_bitmap(gameover, 200, 200, 0);
			al_draw_textf(font, al_map_rgb(255, 255, 255), 80, 150, 0, "YOUR SCORE  : %d ", score);

			al_play_sample(whistle, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			al_flip_display(display);


			break;

		}

		al_flip_display();

		if (ev.type == ALLEGRO_EVENT_TIMER) {

			draw = true;
			if (ev.timer.source == timer2) { //countdown

				timerGame--;

			}
		}





		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {        // Event key is pressed       
			if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				KEY[RIGHT] = true;  // Right key pressed

			}

			else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				KEY[LEFT] = true; // Left key pressed

			}


			if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {

				kicking = true; //Start kick
				al_play_sample(samplekick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			}


			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {

				lives = 0; timerGame = 0;

			}




		}



		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			lives = 0; timerGame = 0;
		}



		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {

				KEY[RIGHT] = false;

			}
			else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				KEY[LEFT] = false;

			}

		}

		// Left and right movement( does not move while kicking)
		if (KEY[LEFT] == true) {

			xpos -= 8;
			if (xpos < 50)
				xpos = 50;
		}
		if (KEY[RIGHT] == true) {
			xpos += 8;
			if (xpos > 750)
				xpos = 750;

		}


		if (kicking == true) { //If kicking
			kicky -= 10;

			r -= 0.15;
			if (kicky < 0 || kicked == true) {

				if (kicked == true) {
					al_stop_samples();
					score++;
					al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);


				}

				else if (kicky < 0)

					lives;
				kicking = false;
				kicked = false;
				kicky = ypos - 25;
				r = 25;
				//al_flush_event_queue(queue);



			}

		}

		if (!kicking) {// To prevent the movement of the ball to the right and left at the time of the kick

			kickx = xpos - 25;
			kicky = ypos - 25;

		}
		if (draw == true) { // Drawing by FPS
			al_clear_to_color(al_map_rgb(193, 255, 255));
			al_draw_bitmap(sky, 0, 0, 0);                 //background
			al_draw_line(0, 100, 800, 100, al_map_rgb(139, 69, 19), 40);
			al_draw_bitmap(background, 0, 120, 0);

			/*			al_draw_line(227, 25, width - 248, 25, al_map_rgb(255, 0, 255), 5);//top
						al_draw_line(230, 24, 230, 180, al_map_rgb(255, 0, 255), 5);//left
						al_draw_line(550, 24, 550, 180, al_map_rgb(255, 0, 255), 5);//right */

			al_draw_bitmap(soccer, 225, 25, 0);
			int i = 0;

			while (i < TOTAL_AIM) {	  //Draw aim(s)

				if (aim[i].active == true) {
					//al_draw_filled_circle(aim[i].x, aim[i].y, 15, al_map_rgb(0, 78, 91)); 
					al_draw_bitmap(taim, aim[i].x, aim[i].y, 15);
				}
				i++;
			}

			int j = 0;
			while (j < GOALKEEPER) {	//Draw goalkeeper

				if (goal[j].active == true) {

					al_draw_bitmap(goalkeep, goal[j].x, goal[j].y, 15);
					moveGoal(goal);
				}
				j++;
			}


			al_draw_filled_circle(xpos, ypos, 25, al_map_rgb(255, 255, 0)); //Position of the ball
			al_draw_line(xpos, ypos - 20, xpos, ypos - 51, al_map_rgb(255, 255, 0), 5);
			al_draw_filled_triangle(xpos, ypos - 71, xpos - 15, ypos - 51, xpos + 15, ypos - 51, al_map_rgb(255, 255, 0));



			//al_draw_filled_circle(kickx, kicky, r, al_map_rgb(255, 127, 0)); //Position of the thrown ball

			al_draw_bitmap(ball, kickx, kicky, r);


			al_draw_line(0, 710, width, 710, al_map_rgb(255, 0, 255), 5);
			al_draw_line(2, 708, 2, 750, al_map_rgb(255, 0, 255), 5);
			al_draw_line(400, 708, 400, 750, al_map_rgb(255, 0, 255), 5);
			al_draw_line(798, 708, 798, 750, al_map_rgb(255, 0, 255), 5);
			al_draw_line(0, 748, width, 748, al_map_rgb(255, 0, 255), 5);
			al_draw_textf(font, al_map_rgb(242, 239, 48), 100, 720, ALLEGRO_ALIGN_LEFT, "SCORE: %d  ", score);
			al_draw_textf(font, al_map_rgb(242, 239, 48), 550, 720, 0, "TIME :%d", timerGame);


			al_flip_display();

			draw = false;

		}

		for (int i = 0; i < TOTAL_AIM; i++) {

			if (controlCrash(aim[i], kickx, kicky) == 1) { //Collision
				aim[i].active = false;  //Reposition aim if collision occurred
				kicked = 1;

				r = 20;

			}

		}

		for (int j = 0; j < GOALKEEPER; j++) {

			if (controlCrashGoal(goal[j], kickx, kicky) == 1) { //Collision
				goal[j].active = false;  //Reposition goalkeeper if collision occurred
				kicking = false;
				kicked = false;
				kicky = ypos - 25;
				r = 20;

			}


		}

	}

	al_rest(2);
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_sample(sample);
	al_destroy_sample(samplekick);
	al_destroy_sample(whistle);

	return 0;
}

/* FREE KICK MODE */
int freekickgame() {

	bool KEY[3] = { false,false,false };

	srand(time(NULL));
	al_init();    //Initialize allegro functions
	al_install_keyboard();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();


	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_TIMER* timer2 = NULL;
	ALLEGRO_BITMAP* background = NULL;
	ALLEGRO_BITMAP* sky = NULL;
	ALLEGRO_BITMAP* soccer = NULL;
	ALLEGRO_BITMAP* taim = NULL;
	ALLEGRO_BITMAP* ball = NULL;
	ALLEGRO_BITMAP* gameover = NULL;
	ALLEGRO_BITMAP* goalkeep = NULL;


	struct item aim[AIM];  //aims
	struct item goal[GOALKEEPER];//goalkeeper
	initAim(aim);
	initGoal(goal);


	ALLEGRO_FONT* font = al_load_font("font/pes.ttf", 20, 0);   //Font size is 20 
	ALLEGRO_FONT* font54 = al_load_font("font/pes.ttf", 54, 0); //Font size is 54
	al_reserve_samples(3);   //By number of sounds
	ALLEGRO_SAMPLE* sample = al_load_sample("sounds/yeehaw.wav");
	ALLEGRO_SAMPLE* samplekick = al_load_sample("sounds/kick.mp3");
	ALLEGRO_SAMPLE* whistle = al_load_sample("sounds/whistle.mp3");


	int xpos = 400, ypos = 670;
	int kickx = xpos, kicky = ypos;
	int score = 0;
	int FPS = 60;

	bool kicking = false; // Is your ball kicking right now?
	bool kicked = false;  // Has the goal been shot?
	bool draw = true;


	display = al_create_display(width, height);  //Display size
	queue = al_create_event_queue();  // for movement


	timer2 = al_create_timer(1); // timer of game
	timer = al_create_timer(1.0 / FPS); //speed
	ALLEGRO_EVENT_QUEUE* queue2 = al_create_event_queue(); //for timer of FPS



	float r = 20;


	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_timer_event_source(timer2));

	//BITMAPS
	background = al_load_bitmap(BACKGROUND_FILE);
	soccer = al_load_bitmap(SOCCER);
	sky = al_load_bitmap(SKY);
	taim = al_load_bitmap(TAIM);
	ball = al_load_bitmap(BALL);
	gameover = al_load_bitmap(GAMEOVER);
	goalkeep = al_load_bitmap(GOALKEEP);

	al_start_timer(timer);
	al_start_timer(timer2);

	while (1) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);
		placeAim(aim);

		if (score > 8)
			placeGoal(goal);

		if (lives == 0 || timerGame == 0) {  //Stop game

			//al_clear_to_color(al_map_rgb(0, 127, 0));
			al_draw_bitmap(background, 0, 0, 0);
			al_draw_bitmap(gameover, 200, 200, 0);
			al_draw_textf(font, al_map_rgb(255, 255, 255), 80, 150, 0, "YOUR SCORE  : %d ", score);

			al_play_sample(whistle, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			al_flip_display(display);


			break;

		}

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			lives = 0; timerGame = 0;
		}


		al_flip_display();

		if (ev.type == ALLEGRO_EVENT_TIMER) {

			draw = true;
			if (ev.timer.source == timer2) {

				timerGame++;

			}
		}

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {            // Event key is pressed   
			if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				KEY[RIGHT] = true; // Right key pressed

			}

			else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				KEY[LEFT] = true; // Left key pressed

			}


			if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {

				kicking = true; //Start kick
				al_play_sample(samplekick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			}




			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {

				lives = 0; timerGame = 0;

			}


		}

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			lives = 0; timerGame = 0;
		}


		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {

				KEY[RIGHT] = false;

			}
			else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				KEY[LEFT] = false;

			}

		}


		// Left and right movement( does not move while kicking)
		if (KEY[LEFT] == true) {


			if (xpos > 50)
				xpos -= 8;

		}

		if (KEY[RIGHT] == true) {

			if (xpos < 750)
				xpos += 8;



		}


		if (kicking == true) {
			kicky -= 10;

			r -= 0.15;
			if (kicky < 0 || kicked == true) {

				if (kicked == true) {
					al_stop_samples();
					score++;
					al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);


				}

				else if (kicky < 0)
					lives--;

				kicking = false;
				kicked = false;
				kicky = ypos - 25;
				r = 25;
				//al_flush_event_queue(queue);



			}

		}

		if (!kicking) {// To prevent the movement of the ball to the right and left at the time of the kick

			kickx = xpos - 25;
			kicky = ypos - 25;

		}
		if (draw == true) { // Drawing by FPS
			al_clear_to_color(al_map_rgb(193, 255, 255));
			al_draw_bitmap(sky, 0, 0, 0); //background
			al_draw_line(0, 100, 800, 100, al_map_rgb(139, 69, 19), 40);
			al_draw_bitmap(background, 0, 120, 0);

			/*	    	al_draw_line(227, 25, width - 248, 25, al_map_rgb(255, 0, 255), 5);//top
						al_draw_line(230, 24, 230, 180, al_map_rgb(255, 0, 255), 5);//left
						al_draw_line(550, 24, 550, 180, al_map_rgb(255, 0, 255), 5);//right */
			al_draw_bitmap(soccer, 225, 25, 0);


			int i = 0;

			while (i < AIM) {	 //Draw aim(s)

				if (aim[i].active == true) {
					//al_draw_filled_circle(aim[i].x, aim[i].y, 15, al_map_rgb(0, 78, 91)); 
					al_draw_bitmap(taim, aim[i].x, aim[i].y, 15);
				}
				i++;
			}

			int j = 0;
			while (j < GOALKEEPER) { //Draw goalkeeper

				if (goal[j].active == true) {
					//al_draw_filled_circle(aim[i].x, aim[i].y, 15, al_map_rgb(0, 78, 91)); 


					al_draw_bitmap(goalkeep, goal[j].x, goal[j].y, 15);
					moveGoal(goal);




				}
				j++;
			}


			al_draw_filled_circle(xpos, ypos, 25, al_map_rgb(255, 255, 0)); //Position of the ball
			al_draw_line(xpos, ypos - 20, xpos, ypos - 51, al_map_rgb(255, 255, 0), 5);
			al_draw_filled_triangle(xpos, ypos - 71, xpos - 15, ypos - 51, xpos + 15, ypos - 51, al_map_rgb(255, 255, 0));

			//al_draw_filled_circle(kickx, kicky, r, al_map_rgb(255, 127, 0));//Position of the thrown ball 

			al_draw_bitmap(ball, kickx, kicky, r);

			al_draw_line(0, 710, width, 710, al_map_rgb(255, 0, 255), 5);
			al_draw_line(2, 708, 2, 750, al_map_rgb(255, 0, 255), 5);
			al_draw_line(400, 708, 400, 750, al_map_rgb(255, 0, 255), 5);
			al_draw_line(798, 708, 798, 750, al_map_rgb(255, 0, 255), 5);
			al_draw_line(0, 748, width, 748, al_map_rgb(255, 0, 255), 5);
			al_draw_textf(font, al_map_rgb(242, 239, 48), 100, 720, ALLEGRO_ALIGN_LEFT, "SCORE: %d  ", score);
			al_draw_textf(font, al_map_rgb(242, 239, 48), 550, 720, 0, "LIVES : %d ", lives);


			al_flip_display();

			draw = false;

		}

		for (int i = 0; i < AIM; i++) {

			if (controlCrash(aim[i], kickx, kicky) == 1) { //Collision 
				aim[i].active = false; //Reposition aim if collision occurred
				kicked = 1;

				r = 20;

			}

		}

		for (int j = 0; j < GOALKEEPER; j++) {

			if (controlCrashGoal(goal[j], kickx, kicky) == 1) { //Collision 
				goal[j].active = false;  //Reposition goalkeeper if collision occurred
				lives--;
				kicking = false;
				kicked = false;
				kicky = ypos - 25;


				r = 20;

			}

		}

		//al_flip_display();
	}

	al_rest(2);
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_sample(sample);
	al_destroy_sample(samplekick);
	al_destroy_sample(whistle);

	return 0;
}




int main() {

	bool KEY[5] = { false,false,false,false,false };


	srand(time(NULL));
	al_init();  //Initialize allegro functions
	al_install_keyboard();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();



	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_BITMAP* background = NULL;
	ALLEGRO_BITMAP* ball = NULL;
	ALLEGRO_BITMAP* menu = NULL;




	struct item aim[TOTAL_AIM]; //Aims
	struct item goal[GOALKEEPER]; //Goalkeeper
	initAim(aim);
	initGoal(goal);



	ALLEGRO_FONT* font34 = al_load_font("font/pes.ttf", 34, 0); //Font size is 34 
	ALLEGRO_FONT* font56 = al_load_font("font/pes.ttf", 56, 0);//Font size is 56
	al_reserve_samples(3); //By number of sounds
	ALLEGRO_SAMPLE* fireball = al_load_sample("sounds/fireball.mp3");
	ALLEGRO_SAMPLE* chosen = al_load_sample("sounds/chosen.mp3");
	ALLEGRO_SAMPLE* nossa = al_load_sample("sounds/nossa.mp3");




	int xpos = 400, ypos = 670;
	int kickx = xpos, kicky = ypos;
	int menux = 150;
	int menuy = 450;
	int score = 0;
	int FPS = 60;

	bool kicking = false;
	bool secim = false;
	bool kicked = false;
	bool draw = true;


	display = al_create_display(width, height);  //Display size
	queue = al_create_event_queue();  // for movement


	timer = al_create_timer(1.0 / FPS); //speed

	float r = 20;


	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));

	//BITMAPS
	background = al_load_bitmap(BACKGROUND_FILE);
	ball = al_load_bitmap(BALL);
	menu = al_load_bitmap(MENU);

	al_start_timer(timer);


	while (1) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue, &ev);

		if (lives == 3 || timerGame == 60) { //Start menu
			//al_play_sample(nossa, 0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			//al_clear_to_color(al_map_rgb(0, 127, 0));
			al_draw_bitmap(background, 0, 0, 0);
			al_draw_bitmap(menu, 70, 25, 0);
			al_draw_textf(font56, al_map_rgb(86, 255, 255), 402, 300, ALLEGRO_ALIGN_CENTRE, "FOOTBALL");


			al_draw_textf(font34, al_map_rgb(255, 255, 170), 200, 460, 0, " Practice Mode");
			al_draw_textf(font34, al_map_rgb(255, 255, 170), 200, 530, 0, " Time Challenge");
			al_draw_textf(font34, al_map_rgb(255, 255, 170), 200, 600, 0, " Free Kick Mode");
			al_draw_textf(font34, al_map_rgb(255, 255, 170), 200, 670, 0, " EXIT");


			al_draw_bitmap(ball, menux, menuy, r);
			al_flip_display(display);

			if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

				if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
					KEY[UP] = true;
					if (menuy > 450) {
						menuy -= 70;
						al_play_sample(fireball, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					}
				}

				else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
					KEY[DOWN] = true;
					if (menuy < 660) {
						menuy += 70;
						printf(" y konum %d \n x konum %d\n\n", menuy, menux);
						printf("**************************\n");  //testing for down pressed
						al_play_sample(fireball, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					}
				}

				else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
					KEY[ENTER] = true;
					secim = true;
					al_play_sample(chosen, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

				}

				if (secim == true)
				{
					if (menuy == 450)
					{
						al_play_sample(chosen, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						practicegame();
						break;
					}
					else if (menuy == 520)
					{
						al_play_sample(chosen, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						timechallengegame();
						break;
					}
					else if (menuy == 590)
					{
						al_play_sample(chosen, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						freekickgame();
						break;
					}
					else
						al_play_sample(chosen, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					al_draw_bitmap(background, 0, 0, 0);
					al_draw_textf(font56, al_map_rgb(255, 255, 255), 220, 350, 0, "BYE BYE ");


					al_flip_display(display);
					al_play_sample(nossa, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					al_rest(10);
					break;
				}


			}


			else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
				if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {

					KEY[UP] = false;

				}
				else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
					KEY[DOWN] = false;

				}

			}

		}


		al_flip_display();
	}

	al_rest(0.00001);
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_sample(chosen);


	return 0;
}




