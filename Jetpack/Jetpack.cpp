// Jetpack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SBDL.h"
#include <iostream>
#include <math.h>
#include <iomanip>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

struct coin {
	int x, y;
	int type;
};
struct line {
	int x1, y1, x2, y2;
};
Texture sector_start, sector1, sector2, lab_start, lab1, lab2, warehouse_Start, warehouse, volcano_start, volcano1, volcano2, load, jetpack, click_toplay;
Texture barry, barry2, barry3, barry4, pause, open_menu, close_menu, menu_bg, distance_texture, best_texture, coin_texture, pause_title, setting_bg, barry_bg;
Texture retry, resume, quit, music_on, music_off, sound_on, sound_off, audio, barry_txt, alien, alien2, alien3, alien4;
Texture coin1, coin2, coin3, coin4, coin5, coin6, lazer_off, lazer_on1, lazer_on2, missle_warning1, missle_warning2, missle1, missle2, missle3, missle4, missle5, missle6;
Texture smoke1, smoke2, smoke3, smoke4, smoke5, smoke6, speed_token1, speed_token2, speed_token3, speed_token4, setting_btn, gameover_title, health, back;
Texture zapper_v1, zapper_v2, zapper_v3, zapper_v4, zapper_h1, zapper_h2, zapper_h3, zapper_h4, zapper_d1, zapper_d2, zapper_d3, zapper_d4, high_bg;
Texture h1_t, h2_t, h3_t, h4_t, h5_t, h6_t, h7_t, h8_t, h9_t, h10_t, parallax_front1, parallax_front2, parallax_back1;
Font *jetpackia30, *jetpackia20;
Music *bg_music;
Sound *jet_sound, *laser_warning, *lazeron, *run3, *missle_warning, *missle_launch, *coin_sound, *health_sound, *boost_sound, *zapper_sound;
string s[10];
coin coin0;
line line1, line2, line3;
SDL_Rect barry_rect;
ofstream high_w;
const int window_width = 1000, window_height = 460;
int bg_x, bg_type = 1, menu_x = window_width, rec_x, jetpack_x = window_width / 2 - 200, close_menu_x = window_width - 45, barry_type = 1;
int load_timer = 400, load_length = 2, click_toplay_timer = 70, barry_timer = 12, timer_distance = 40, distanc, best_score, coins, coin_timer = 10;
int lazer_timer = 300, lazer_timer2 = 600, missle_timer = 350, missle_type = 1, x_missle = window_width, i, timer_rotation = 10, speed_token_timer = 15, speed_token_type = 1;
int i_speed_token = 1, x_speed_token = window_width, y_speed_token = rand() % 200 + 130, lazer_pattern, coin_pattern, x_coin = window_width, y_coin, y_lazer;
int x_z1, x_z2, x_z3, y_z1, y_z2, y_z3, z1_type, z2_type, z3_type, z1_t, z2_t, z3_t, zapper_timer = 10;
int h1, h2, h3, h4, h5, h6, h7, h8, h9, speeed_token_timer = 150, para_x = window_width + 50;
bool start_menu = true, pause_menu, menu_open, menu_close, play_music = true, play_sound = true, music_is_playing, setting, lazer_type, lazer_on;
bool lazer_sound, walk_sound, missle_sound, up = true, down, speedToken, lazer, missil, sh_coin, gameover, show_zapper, highscore, boost, w, para;
double v_barry = -0.5, a_barry = 10, y_barry = window_height - barry2.height - 110, y_missle = y_barry, v_bg = 4;
char ch;
coin** coin_ar;
void load_assets();
void background();
void StartMenu();
void LoadPage();
void Game();
void PauseMenu();
void show_coin();
void settingPage();
void lazer1();
void lazer2();
void lazer3();
void missle();
void read_pattern();
void speed_token();
void gameover_page();
void zapper();
void highscores_page();
void write_high();
void read_high();

int main(int argc, char *argv[])
{
	SBDL::InitEngine("Jetpack", window_width, window_height);
	const int FPS = 60; //frame per second
	const int delay = 1000 / FPS; //delay we need at each frame
	load_assets();
	para_x = volcano2.width;
	read_high();
	ifstream coin_r;
	coin_r.open("coins.txt");
	coin_r >> coins;
	coin_r.close();
	while (SBDL::isRunning()) {
		srand(time(NULL));
		int startTime = SBDL::getTime();
		SBDL::updateEvents();
		SBDL::clearRenderScreen();

		if (load_timer)
			LoadPage();
		else {
			background();
			if (start_menu)
				StartMenu();
			else {
				if (speedToken)
					speed_token();
				else if (distanc % 500 == 0 && distanc != 0) {
					x_speed_token = window_width;
					y_speed_token = rand() % 200 + 130;
					speedToken = true;
				}
				if (lazer) {
					if (lazer_pattern == 0)
						lazer1();
					else if (lazer_pattern == 1)
						lazer2();
					else
						lazer3();
				}
				else if (distanc % 150 == 0 && distanc != 0) {
					lazer = true;
					lazer_pattern = rand() % 3;
					lazer_timer = 300;
					lazer_timer2 = 600;
				}
				if (missil)
					missle();
				else if (distanc % 100 == 0 && distanc != 0 && !lazer) {
					missil = true;
					x_missle = window_width;
					y_missle = y_barry;
					missle_timer = 350;
					y_lazer = rand() % 200 + 130;
				}
				if (boost) {
					missil = false;
					lazer = false;
					sh_coin = false;
					speedToken = false;
					show_zapper = false;
					v_bg = 20;
				}
				if (!pause_menu && !gameover) {
					speed_token_timer--;
					if (speed_token_timer <= 0) {
						boost = false;
						v_bg = 4;
					}
				}
				if (sh_coin)
					show_coin();
				else if (distanc % 50 == 0 && distanc != 0 && !lazer) {
					coin_pattern = rand() % 5;
					sh_coin = true;
					y_coin = rand() % 250 + 30;
					x_coin = window_width;
					read_pattern();
				}
				if (show_zapper)
					zapper();
				else if (!sh_coin && !lazer && distanc % 150 <= 80 && distanc % 50 <= 37) {
					show_zapper = true;
					x_z1 = window_width;
					x_z2 = x_z1 + 400;
					x_z3 = x_z2 + 400;
					z1_t = rand() % 3;
					if (z1_t == 0)
						y_z1 = rand() % 340 + 30;
					else
						y_z1 = rand() % 200 + 30;
					z2_t = rand() % 3;
					if (z2_t == 0)
						y_z2 = rand() % 340 + 30;
					else
						y_z2 = rand() % 200 + 30;
					z3_t = rand() % 3;
					if (z3_t == 0)
						y_z3 = rand() % 340 + 30;
					else
						y_z3 = rand() % 200 + 30;
				}
				Game();
				if (gameover)
					gameover_page();
				else if (pause_menu)
					PauseMenu();
			}
		}
		if (!play_music) {
			SBDL::stopMusic();
			music_is_playing = false;
		}
		SBDL::updateRenderScreen();
		int elapsedTime = SBDL::getTime() - startTime;
		if (elapsedTime < delay)
			SBDL::delay(delay - elapsedTime);
	}
}

void load_assets() {
	sector_start = SBDL::loadTexture("assets/pic/back/Sector Start.png");
	sector1 = SBDL::loadTexture("assets/pic/back/Sector1.png");
	sector2 = SBDL::loadTexture("assets/pic/back/Sector 2.png");
	lab_start = SBDL::loadTexture("assets/pic/back/Lab Start.png");
	lab1 = SBDL::loadTexture("assets/pic/back/Lab1.png");
	lab2 = SBDL::loadTexture("assets/pic/back/Lab2.png");
	warehouse_Start = SBDL::loadTexture("assets/pic/back/warehouse Start.png");
	warehouse = SBDL::loadTexture("assets/pic/back/warehouse.png");
	volcano_start = SBDL::loadTexture("assets/pic/back/Volcano Start.png");
	volcano1 = SBDL::loadTexture("assets/pic/back/Volcano1.png");
	volcano2 = SBDL::loadTexture("assets/pic/back/Volcano2.png");
	load = SBDL::loadTexture("assets/pic/start.png");
	jetpack = SBDL::loadTexture("assets/pic/menu/pausebg.png");
	jetpackia30 = SBDL::loadFont("assets/font/Jetpackia.ttf", 30);
	jetpackia20 = SBDL::loadFont("assets/font/Jetpackia.ttf", 20);
	click_toplay = SBDL::loadTexture("assets/pic/menu/click_toplay.png");
	barry = SBDL::loadTexture("assets/pic/barry/barry_.png");
	barry2 = SBDL::loadTexture("assets/pic/barry/barry_2.png");
	barry3 = SBDL::loadTexture("assets/pic/barry/barry_3.png");
	barry4 = SBDL::loadTexture("assets/pic/barry/barry_4.png");
	pause = SBDL::loadTexture("assets/pic/menu/pause.png");
	open_menu = SBDL::loadTexture("assets/pic/menu/open1.png");
	close_menu = SBDL::loadTexture("assets/pic/menu/close1.png");
	menu_bg = SBDL::loadTexture("assets/pic/menu/menubg.png");
	coin1 = SBDL::loadTexture("assets/pic/coin/1.1.png");
	coin2 = SBDL::loadTexture("assets/pic/coin/2.1.png");
	coin3 = SBDL::loadTexture("assets/pic/coin/3.1.png");
	coin4 = SBDL::loadTexture("assets/pic/coin/4.1.png");
	coin5 = SBDL::loadTexture("assets/pic/coin/5.1.png");
	coin6 = SBDL::loadTexture("assets/pic/coin/6.1.png");
	bg_music = SBDL::loadMusic("assets/sound/bg_music.wav");
	jet_sound = SBDL::loadSound("assets/sound/jetpack_jet_lp-[AudioTrimmer.com] .wav");
	pause_title = SBDL::loadTexture("assets/pic/menu/pause title2.png");
	retry = SBDL::loadTexture("assets/pic/menu/retry.png");
	resume = SBDL::loadTexture("assets/pic/menu/resume.png");
	quit = SBDL::loadTexture("assets/pic/menu/quit.png");
	music_on = SBDL::loadTexture("assets/pic/menu/music on.jpg");
	music_off = SBDL::loadTexture("assets/pic/menu/music off.jpg");
	sound_on = SBDL::loadTexture("assets/pic/menu/sound on.jpg");
	sound_off = SBDL::loadTexture("assets/pic/menu/sound off.jpg");
	setting_bg = SBDL::loadTexture("assets/pic/menu/setting bg2.png");
	audio = SBDL::loadTexture("assets/pic/menu/audio.png");
	barry_txt = SBDL::loadTexture("assets/pic/menu/barry.png");
	alien = SBDL::loadTexture("assets/pic/barry/alien .png");
	alien2 = SBDL::loadTexture("assets/pic/barry/alien2 .png");
	alien3 = SBDL::loadTexture("assets/pic/barry/alienup .png");
	alien4 = SBDL::loadTexture("assets/pic/barry/aliendown .png");
	barry_bg = SBDL::loadTexture("assets/pic/menu/barry_bg.png");
	lazer_off = SBDL::loadTexture("assets/pic/lazer/laser_noneactive.png");
	lazer_on1 = SBDL::loadTexture("assets/pic/lazer/laser_active2.png");
	lazer_on2 = SBDL::loadTexture("assets/pic/lazer/laser_active1.png");
	laser_warning = SBDL::loadSound("assets/sound/laser_warning.wav");
	lazeron = SBDL::loadSound("assets/sound/laser_fire_lp.wav");
	run3 = SBDL::loadSound("assets/sound/boots_step_right_3.wav");
	missle_warning1 = SBDL::loadTexture("assets/pic/missle/1m1.png");
	missle_warning2 = SBDL::loadTexture("assets/pic/missle/2m2.png");
	missle1 = SBDL::loadTexture("assets/pic/missle/missle(1).png");
	missle2 = SBDL::loadTexture("assets/pic/missle/missle(2).png");
	missle3 = SBDL::loadTexture("assets/pic/missle/missle(3).png");
	missle4 = SBDL::loadTexture("assets/pic/missle/missle(4).png");
	missle5 = SBDL::loadTexture("assets/pic/missle/missle(5).png");
	missle6 = SBDL::loadTexture("assets/pic/missle/missle(6).png");
	missle_warning = SBDL::loadSound("assets/sound/missile_warning.wav");
	missle_launch = SBDL::loadSound("assets/sound/missile_launch.wav");
	smoke1 = SBDL::loadTexture("assets/pic/smoke/smoke 1.png");
	smoke2 = SBDL::loadTexture("assets/pic/smoke/smoke 2.png");
	smoke3 = SBDL::loadTexture("assets/pic/smoke/smoke 3.png");
	smoke4 = SBDL::loadTexture("assets/pic/smoke/smoke 4.png");
	smoke5 = SBDL::loadTexture("assets/pic/smoke/smoke 5.png");
	smoke6 = SBDL::loadTexture("assets/pic/smoke/smoke 6.png");
	speed_token1 = SBDL::loadTexture("assets/pic/speedToken/speed token.png");
	speed_token2 = SBDL::loadTexture("assets/pic/speedToken/speed token2.png");
	speed_token3 = SBDL::loadTexture("assets/pic/speedToken/speed token3.png");
	speed_token4 = SBDL::loadTexture("assets/pic/speedToken/speed token4.png");
	setting_btn = SBDL::loadTexture("assets/pic/menu/setting.png");
	gameover_title = SBDL::loadTexture("assets/pic/menu/gameover title.png");
	health = SBDL::loadTexture("assets/pic/menu/resume 500.png");
	coin_sound = SBDL::loadSound("assets/sound/coin.wav");
	back = SBDL::loadTexture("assets/pic/menu/back.png");
	zapper_d1 = SBDL::loadTexture("assets/pic/zappers/d1.png");
	zapper_d2 = SBDL::loadTexture("assets/pic/zappers/d2.png");
	zapper_d3 = SBDL::loadTexture("assets/pic/zappers/d3.png");
	zapper_d4 = SBDL::loadTexture("assets/pic/zappers/d4.png");
	zapper_h1 = SBDL::loadTexture("assets/pic/zappers/h1.png");
	zapper_h2 = SBDL::loadTexture("assets/pic/zappers/h2.png");
	zapper_h3 = SBDL::loadTexture("assets/pic/zappers/h3.png");
	zapper_h4 = SBDL::loadTexture("assets/pic/zappers/h4.png");
	zapper_v1 = SBDL::loadTexture("assets/pic/zappers/v1.png");
	zapper_v2 = SBDL::loadTexture("assets/pic/zappers/v2.png");
	zapper_v3 = SBDL::loadTexture("assets/pic/zappers/v3.png");
	zapper_v4 = SBDL::loadTexture("assets/pic/zappers/v4.png");
	high_bg = SBDL::loadTexture("assets/pic/menu/high bg2.png");
	health_sound = SBDL::loadSound("assets/sound/soundonEffect.wav");
	boost_sound = SBDL::loadSound("assets/sound/boost_boost.wav");
	zapper_sound = SBDL::loadSound("assets/sound/zapper.wav");
	parallax_back1 = SBDL::loadTexture("assets/pic/parallex/Second P.png");
	parallax_front1 = SBDL::loadTexture("assets/pic/parallex/First P.png");
	parallax_front2 = SBDL::loadTexture("assets/pic/parallex/First P2.png");
}

void background() {
	if (bg_type == 1) {
		SDL_Rect sector_start_rect = { bg_x, 0,  sector_start.width ,window_height };
		SDL_Rect sector1_rect = { sector_start.width + bg_x, 0,  sector1.width ,window_height };
		SBDL::showTexture(sector_start, sector_start_rect);
		SBDL::showTexture(sector1, sector1_rect);
		if (bg_x <= -sector_start.width) {
			bg_type = 2;
			bg_x += sector_start.width;
		}
	}
	else if (bg_type == 2) {
		SDL_Rect sector1_rect = { bg_x, 0,  sector1.width ,window_height };
		SDL_Rect sector2_rect = { sector1.width + bg_x , 0,  sector2.width ,window_height };
		SBDL::showTexture(sector1, sector1_rect);
		SBDL::showTexture(sector2, sector2_rect);
		if (bg_x <= -sector1.width) {
			bg_type = 3;
			bg_x += sector1.width;
		}
	}
	else if (bg_type == 3) {
		SDL_Rect sector2_rect = { bg_x , 0,  sector2.width ,window_height };
		SDL_Rect lab_start_rect = { sector2.width + bg_x , 0,  sector1.width ,window_height };
		SBDL::showTexture(sector2, sector2_rect);
		SBDL::showTexture(lab_start, lab_start_rect);
		if (bg_x <= -sector2.width) {
			bg_type = 4;
			bg_x += sector2.width;
		}
	}
	else if (bg_type == 4) {
		SDL_Rect lab_start_rect = { bg_x , 0,  sector1.width ,window_height };
		SDL_Rect lab1_rect = { lab_start.width + bg_x, 0,  sector1.width ,window_height };
		SBDL::showTexture(lab_start, lab_start_rect);
		SBDL::showTexture(lab1, lab1_rect);
		if (bg_x <= -lab_start.width) {
			bg_type = 5;
			bg_x += lab_start.width;
		}
	}
	else if (bg_type == 5) {
		SDL_Rect lab1_rect = { bg_x , 0,  sector1.width ,window_height };
		SDL_Rect lab2_rect = { lab1.width + bg_x, 0,  sector1.width ,window_height };
		SBDL::showTexture(lab1, lab1_rect);
		SBDL::showTexture(lab2, lab2_rect);
		if (bg_x <= -lab1.width) {
			bg_type = 6;
			bg_x += lab1.width;
		}
	}
	else if (bg_type == 6) {
		SDL_Rect lab2_rect = { bg_x, 0,  sector1.width ,window_height };
		SDL_Rect volcano_start_rect = { lab2.width + bg_x, 0,  sector1.width ,window_height };
		SBDL::showTexture(lab2, lab2_rect);
		SBDL::showTexture(volcano_start, volcano_start_rect);
		if (bg_x <= -lab2.width) {
			bg_type = 7;
			bg_x += lab2.width;
		}
	}
	else if (bg_type == 7) {
		SDL_Rect volcano_start_rect = { bg_x, 0,  sector1.width ,window_height };
		SDL_Rect volcano1_rect = { volcano_start.width + bg_x, 0,  sector1.width ,window_height };
		SBDL::showTexture(volcano_start, volcano_start_rect);
		SBDL::showTexture(volcano1, volcano1_rect);
		if (bg_x <= -volcano_start.width) {
			bg_type = 8;
			bg_x += volcano_start.width;
		}
	}
	else if (bg_type == 8) {
		SDL_Rect volcano1_rect = { bg_x, 0,  sector1.width ,window_height };
		SDL_Rect volcano2_rect = { volcano1.width + bg_x, 0,  sector1.width ,window_height };
		SBDL::showTexture(volcano1, volcano1_rect);
		SBDL::showTexture(volcano2, volcano2_rect);
		if (bg_x <= -volcano1.width) {
			bg_type = 9;
			bg_x += volcano1.width;
		}
	}
	else if (bg_type == 9) {
		SDL_Rect volcano2_rect = { bg_x, 0,  sector1.width ,window_height };
		SDL_Rect warehouse_Start_rect = { volcano1.width + bg_x, 0,  sector1.width ,window_height };
		SDL_Rect para_front_rect = { volcano1.width + bg_x + 60, 50, parallax_front1.width - 100, parallax_front1.height - 50 };
		SDL_Rect para_back_rect = { para_x, 130, parallax_back1.width + 100, parallax_back1.height };
		SBDL::showTexture(volcano2, volcano2_rect);
		SBDL::showTexture(warehouse_Start, warehouse_Start_rect);
		SBDL::showTexture(parallax_back1, para_back_rect);
		SBDL::showTexture(parallax_front1, para_front_rect);
		if (bg_x <= -volcano2.width) {
			bg_type = 10;
			bg_x += volcano2.width;
		}
		if (bg_x <= volcano2.width - window_width && !para) {
			para_x = 2 * window_width;
			para = true;
		}
	}
	else if (bg_type == 10) {
		SDL_Rect warehouse_Start_rect = { bg_x, 0,  sector1.width ,window_height };
		SDL_Rect warehouse_rect = { warehouse_Start.width + bg_x, 0,  sector1.width ,window_height };
		SDL_Rect para_front1_rect = { bg_x + 60, 50, parallax_front1.width - 100, parallax_front1.height - 50 };
		SDL_Rect para_front2_rect = { warehouse_Start.width + bg_x + 60, 50, parallax_front2.width - 100, parallax_front2.height - 50 };
		SDL_Rect para_back_rect = { para_x, 130, parallax_back1.width + 100, parallax_back1.height };
		SBDL::showTexture(warehouse_Start, warehouse_Start_rect);
		SBDL::showTexture(warehouse, warehouse_rect);
		SBDL::showTexture(parallax_back1, para_back_rect);
		SBDL::showTexture(parallax_front1, para_front1_rect);
		SBDL::showTexture(parallax_front1, para_front2_rect);
		if (bg_x <= -warehouse_Start.width) {
			bg_type = 11;
			bg_x += warehouse_Start.width;
		}
	}
	else if (bg_type == 11) {
		SDL_Rect warehouse_rect = { bg_x, 0,  sector1.width ,window_height };
		SDL_Rect sector_start_rect = { warehouse.width + bg_x, 0,  sector_start.width ,window_height };
		SDL_Rect para_front1_rect = { bg_x + 60, 50, parallax_front1.width - 100, parallax_front1.height - 50 };
		SDL_Rect para_back_rect = { para_x, 130, parallax_back1.width + 100, parallax_back1.height };
		SBDL::showTexture(warehouse, warehouse_rect);
		SBDL::showTexture(sector_start, sector_start_rect);
		SBDL::showTexture(parallax_back1, para_back_rect);
		SBDL::showTexture(parallax_front1, para_front1_rect);
		if (bg_x <= -warehouse.width) {
			bg_type = 1;
			para_x = volcano1.width + bg_x;
			bg_x += warehouse.width;
		}
	}
	if (!start_menu && !pause_menu && !gameover) {
		bg_x -= v_bg;
		if (para_x > -2047)
			para_x -= 3 * v_bg / 4;
		else
			para = false;
	}
}

void StartMenu() {
	SDL_Rect rec = { rec_x, 0, window_width, window_height };
	SDL_Rect open_menu_rect = { window_width - open_menu.width, 10, open_menu.width, open_menu.height };
	SDL_Rect setting_rect = { window_width - 300, 25, 280, 60 };
	SDL_Rect highscore_rect = { window_width - 300, 100, 280, 60 };
	SDL_Rect quit_rect = { window_width - 300, 180, 280, 60 };
	if (barry_type == 1)
		SBDL::showTexture(barry2, 20, y_barry);
	else
		SBDL::showTexture(alien2, 20, y_barry);
	SBDL::drawRectangle(rec, 0, 0, 0, 150);
	SBDL::showTexture(jetpack, jetpack_x, window_height / 2 - (jetpack.height / 2) - 10);
	if (menu_open) {
		if (menu_x > window_width - 330) {
			menu_x -= 11;
			jetpack_x -= 5;
			rec_x -= 11;
		}
		SBDL::showTexture(close_menu, menu_x - close_menu.width, 10);
		if (SBDL::mouseInRect(rec) && SBDL::Mouse.clicked() && !setting && !highscore) {
			menu_close = true;
			menu_open = false;
		}
	}
	else if (menu_close) {
		if (menu_x < window_width) {
			menu_x += 11;
			jetpack_x += 5;
			rec_x += 11;
		}
		else if (menu_x >= window_width)
			menu_close = false;
		SBDL::showTexture(open_menu, menu_x - open_menu.width, 10);
	}
	else {
		SBDL::showTexture(open_menu, open_menu_rect);
		if (click_toplay_timer > 25)
			SBDL::showTexture(click_toplay, window_width / 2 - click_toplay.width / 2, window_height - 55);
		else if (click_toplay_timer == 0)
			click_toplay_timer = 70;
		click_toplay_timer--;
		if (SBDL::mouseInRect(rec) && SBDL::Mouse.clicked() && !SBDL::mouseInRect(open_menu_rect) && !menu_open && !setting && !highscore)
			start_menu = false;
	}
	if (SBDL::mouseInRect(setting_rect) && SBDL::Mouse.clicked() && menu_open)
		setting = true;
	if (SBDL::mouseInRect(highscore_rect) && SBDL::Mouse.clicked() && menu_open)
		highscore = true;
	/*if (SBDL::mouseInRect(quit_rect) && SBDL::Mouse.clicked() && menu_open)
	exit;*/
	if (SBDL::mouseInRect(open_menu_rect) && SBDL::Mouse.clicked())
		menu_open = true;
	SBDL::showTexture(menu_bg, menu_x, 0);
	if (setting)
		settingPage();
	if (highscore)
		highscores_page();
}

void LoadPage() {
	SDL_Rect load_rect = { 0, 0, window_width, window_height };
	SDL_Rect loading_rec = { 310,345,load_length,10 };
	load_length += 1;
	SBDL::showTexture(load, load_rect);
	SBDL::drawRectangle(loading_rec, 250, 250, 250);
	load_timer--;
}

void Game() {
	if (!music_is_playing && play_music) {
		SBDL::playMusic(bg_music, -1);
		music_is_playing = true;
	}
	SDL_Rect rect = { 0, 0, window_width, window_height };
	if (SBDL::keyHeld(SDL_SCANCODE_SPACE)) {
		//SBDL::playSound(jet_sound, 1);
		barry_rect = { 100 , int(y_barry) , 71, 84 };
		if (barry_type == 1)
			SBDL::showTexture(barry3, 100, y_barry);
		else
			SBDL::showTexture(alien3, 100, y_barry);
		if (y_barry > 20 && !pause_menu && !gameover && !boost) {
			y_barry += v_barry;
			v_barry -= a_barry / 30;
		}
	}
	else if (y_barry < window_height - barry2.height - 20) {
		barry_rect = { 100 , int(y_barry) , 71, 84 };
		if (barry_type == 1)
			SBDL::showTexture(barry4, 100, y_barry);
		else
			SBDL::showTexture(alien4, 100, y_barry);
		if (!pause_menu && !gameover && !boost) {
			y_barry += v_barry;
			v_barry += a_barry / 30;
		}
	}
	else if (y_barry >= window_height - barry2.height - 20) {
		v_barry = -1;
		if (barry_timer > 5) {
			if (play_sound && !pause_menu && !walk_sound && !gameover) {
				SBDL::playSound(run3, 1);
				walk_sound = true;
			}
			barry_rect = { 100 , int(y_barry) , 71, 84 };
			if (barry_type == 1)
				SBDL::showTexture(barry2, 100, y_barry);
			else
				SBDL::showTexture(alien2, 100, y_barry);
		}
		else {
			walk_sound = false;
			barry_rect = { 100 , int(y_barry) , 71, 84 };
			if (barry_type == 1)
				SBDL::showTexture(barry, 100, y_barry);
			else
				SBDL::showTexture(alien, 100, y_barry);
		}
	}
	if (y_barry <= 20 && !pause_menu && !gameover && !boost) {
		y_barry = 20;
		v_barry = 0.5;
	}
	if (y_barry >= window_height - barry2.height - 20 && !pause_menu && !gameover && !boost) {
		y_barry = window_height - barry2.height - 20;
		v_barry = -0.5;
	}
	SDL_Rect pause_rect = { window_width - pause.width - 10, 10, pause.width, pause.height };
	SBDL::showTexture(pause, pause_rect);
	if (SBDL::mouseInRect(pause_rect) && SBDL::Mouse.clicked())
		pause_menu = true;
	if (barry_timer == 0)
		barry_timer = 12;
	if (!pause_menu && !gameover && !boost)
		barry_timer--;
	string s;
	if (distanc < 10)
		s = "000";
	else if (distanc < 100)
		s = "00";
	else if (distanc < 1000)
		s = "0";
	SBDL::freeTexture(distance_texture);
	SBDL::freeTexture(best_texture);
	SBDL::freeTexture(coin_texture);
	distance_texture = SBDL::createFontTexture(jetpackia30, s + to_string(distanc) + " m", 171, 171, 171);
	best_texture = SBDL::createFontTexture(jetpackia20, "best: " + to_string(best_score), 171, 171, 171);
	coin_texture = SBDL::createFontTexture(jetpackia20, to_string(coins), 254, 254, 60);
	SDL_Rect dis_rect = { 8 , 10, 95, distance_texture.height };
	SDL_Rect best_rect = { 8 , 12 + distance_texture.height, best_texture.width + 5, best_texture.height };
	SDL_Rect coins_rect = { 8 , 14 + distance_texture.height + best_texture.height, coin_texture.width + 30, coin_texture.height };
	SBDL::drawRectangle(dis_rect, 0, 0, 0, 150);
	SBDL::drawRectangle(best_rect, 0, 0, 0, 150);
	SBDL::drawRectangle(coins_rect, 0, 0, 0, 150);
	SBDL::showTexture(distance_texture, 10, 10);
	SBDL::showTexture(best_texture, 10, 12 + distance_texture.height);
	SBDL::showTexture(coin1, 10, 16 + best_texture.height + distance_texture.height);
	SBDL::showTexture(coin_texture, 15 + coin1.width, 14 + best_texture.height + distance_texture.height);
	if (timer_distance > 0 && !pause_menu && !gameover)
		timer_distance -= v_bg;
	else if (!pause_menu && !gameover) {
		timer_distance = 40;
		distanc++;
	}
}

void PauseMenu() {
	SDL_Rect rec = { rec_x, 0, window_width, window_height };
	SDL_Rect retry_rect = { window_width / 2 - 100, window_height / 2 - 50, 200, 50 };
	SDL_Rect resume_rect = { window_width / 2 + 120, window_height / 2 - 50, 200, 50 };
	SDL_Rect quit_rect = { window_width / 2 - 320, window_height / 2 - 50, 200, 50 };
	SDL_Rect setting_rect = { window_width / 2 - 100, window_height / 2 + 30 , 200, 50 };
	SBDL::drawRectangle(rec, 0, 0, 0, 150);
	SBDL::showTexture(pause_title, 0, -3);
	SBDL::showTexture(retry, retry_rect);
	SBDL::showTexture(resume, resume_rect);
	SBDL::showTexture(quit, quit_rect);
	SBDL::showTexture(setting_btn, setting_rect);
	if (setting)
		settingPage();
	else {
		if (SBDL::mouseInRect(setting_rect) && SBDL::Mouse.clicked())
			setting = true;
		if (SBDL::mouseInRect(resume_rect) && SBDL::Mouse.clicked())
			pause_menu = false;
		if (SBDL::mouseInRect(retry_rect) && SBDL::Mouse.clicked()) {
			pause_menu = false;
			y_barry = window_height - barry2.height - 20;
			barry_timer = 10;
			distanc = 0;
			bg_type = 1;
			bg_x = 0;
			speedToken = false;
			lazer = false;
			missil = false;
			sh_coin = false;
			show_zapper = false;
		}
		if (SBDL::mouseInRect(quit_rect) && SBDL::Mouse.clicked()) {
			pause_menu = false;
			y_barry = window_height - barry2.height - 20;
			barry_timer = 10;
			distanc = 0;
			bg_type = 1;
			bg_x = 0;
			start_menu = true;
			SBDL::stopMusic();
			music_is_playing = false;
			speedToken = false;
			lazer = false;
			missil = false;
			sh_coin = false;
			show_zapper = false;
		}
	}
}

void show_coin() {
	SDL_Rect coin_rect;
	for (int k = 0; k < i - 1; k++) {
		for (int j = 0; j < s[k].size(); j++) {
			if (coin_ar[k][j].type != 0) {
				coin_rect = { coin_ar[k][j].x, coin_ar[k][j].y, 20, 20 };
				if (SBDL::hasIntersectionRect(barry_rect, coin_rect)) {
					coin_ar[k][j].type = 0;
					SBDL::playSound(coin_sound, 1);
					coins++;
				}
				if (coin_ar[k][j].type == 1)
					SBDL::showTexture(coin1, coin_ar[k][j].x, coin_ar[k][j].y);
				else if (coin_ar[k][j].type == 2)
					SBDL::showTexture(coin2, coin_ar[k][j].x, coin_ar[k][j].y);
				else if (coin_ar[k][j].type == 3)
					SBDL::showTexture(coin3, coin_ar[k][j].x, coin_ar[k][j].y);
				else if (coin_ar[k][j].type == 4)
					SBDL::showTexture(coin4, coin_ar[k][j].x, coin_ar[k][j].y);
				else if (coin_ar[k][j].type == 5)
					SBDL::showTexture(coin5, coin_ar[k][j].x, coin_ar[k][j].y);
				else if (coin_ar[k][j].type == 6)
					SBDL::showTexture(coin6, coin_ar[k][j].x, coin_ar[k][j].y);
			}
			if (!pause_menu && !gameover)
				coin_ar[k][j].x -= v_bg;
		}
	}
	if (timer_rotation == 0) {
		timer_rotation = 10;
		for (int k = 0; k < i - 1; k++) {
			for (int j = 0; j < s[k].size(); j++) {
				if (coin_ar[k][j].type != 0 && !pause_menu && !gameover) {
					coin_ar[k][j].type += 1;
					if (coin_ar[k][j].type > 6)
						coin_ar[k][j].type = 1;
				}
			}
		}
	}
	if (coin_ar[0][s[0].size() - 1].x <= 0) {
		sh_coin = false;
		for (int k = 0; k < i - 1; k++)
			delete[] coin_ar[k];
		delete[]coin_ar;
	}
	timer_rotation--;
}

void settingPage() {
	SDL_Rect rec = { 0, 0, window_width, window_height };
	SDL_Rect close_rect = { window_width / 2 + setting_bg.width / 2 - 60, 20, 45, 45 };
	SDL_Rect music_rect = { window_width / 2 - music_on.width / 2 - 5, 160, 50,50 };
	SDL_Rect sound_rect = { window_width / 2 + 5, 160, 50, 50 };
	SDL_Rect barry_rect = { window_width / 2 - barry2.width - 20, 300, barry2.width, barry2.height };
	SDL_Rect alien_rect = { window_width / 2 + 20, 300, alien2.width, alien2.height };
	SBDL::drawRectangle(rec, 0, 0, 0, 200);
	SBDL::showTexture(setting_bg, window_width / 2 - setting_bg.width / 2, 5);
	SBDL::showTexture(audio, window_width / 2 - audio.width / 2, 100);
	SBDL::showTexture(barry_txt, window_width / 2 - barry_txt.width / 2, 230);
	if (barry_type == 1)
		SBDL::showTexture(barry_bg, window_width / 2 - barry_bg.width - 22, 298);
	else
		SBDL::showTexture(barry_bg, window_width / 2 + 18, 298);
	SBDL::showTexture(barry2, barry_rect);
	SBDL::showTexture(alien2, alien_rect);
	if (play_music)
		SBDL::showTexture(music_on, music_rect);
	else
		SBDL::showTexture(music_off, music_rect);
	if (play_sound)
		SBDL::showTexture(sound_on, sound_rect);
	else
		SBDL::showTexture(sound_off, sound_rect);
	if (SBDL::mouseInRect(close_rect) && SBDL::Mouse.clicked())
		setting = false;
	if (SBDL::mouseInRect(music_rect) && SBDL::Mouse.clicked())
		play_music = !play_music;
	if (SBDL::mouseInRect(sound_rect) && SBDL::Mouse.clicked())
		play_sound = !play_sound;
	if (SBDL::mouseInRect(alien_rect) && SBDL::Mouse.clicked())
		barry_type = 2;
	if (SBDL::mouseInRect(barry_rect) && SBDL::Mouse.clicked())
		barry_type = 1;
}

void lazer1() {
	SDL_Rect lazer_rect = { 0, y_lazer, window_width, 45 };
	if (lazer_timer > 200) {
		SBDL::showTexture(lazer_off, lazer_rect);
		if (play_sound && !lazer_sound) {
			SBDL::playSound(laser_warning, 1);
			lazer_sound = true;
		}
	}
	else if (lazer_timer > 0) {
		if (!lazer_sound && play_sound) {
			SBDL::playSound(lazeron, 3);
			lazer_sound = true;
		}
		if (lazer_type)
			SBDL::showTexture(lazer_on1, lazer_rect);
		else
			SBDL::showTexture(lazer_on2, lazer_rect);
		if (lazer_timer % 10 == 0)
			lazer_type = !lazer_type;
	}
	if (!pause_menu && !gameover)
		lazer_timer--;
	if (lazer_timer == 200 || lazer_timer == 0)
		lazer_sound = false;
	if (lazer_timer == 0)
		lazer = false;
	if (SBDL::hasIntersectionRect(barry_rect, lazer_rect) && lazer_timer < 200 && !gameover) {
		gameover = true;
		w = false;
	}
}

void lazer2() {
	SDL_Rect lazer_rect = { 0, 15, window_width, 45 };
	SDL_Rect lazer_rect1 = { 0, 105, window_width, 45 };
	SDL_Rect lazer_rect2 = { 0, 200, window_width, 45 };
	SDL_Rect lazer_rect3 = { 0, 300, window_width, 45 };
	SDL_Rect lazer_rect4 = { 0, 400, window_width, 45 };
	if (lazer_timer2 > 500) {
		SBDL::showTexture(lazer_off, lazer_rect);
		SBDL::showTexture(lazer_off, lazer_rect1);
		SBDL::showTexture(lazer_off, lazer_rect2);
		if (play_sound && !lazer_sound) {
			SBDL::playSound(laser_warning, 1);
			lazer_sound = true;
		}
	}
	else if (lazer_timer2 > 300) {
		if (!lazer_sound && play_sound) {
			SBDL::playSound(lazeron, 3);
			lazer_sound = true;
		}
		if (lazer_type) {
			SBDL::showTexture(lazer_on1, lazer_rect);
			SBDL::showTexture(lazer_on1, lazer_rect1);
			SBDL::showTexture(lazer_on1, lazer_rect2);
			SBDL::showTexture(lazer_off, lazer_rect3);
			SBDL::showTexture(lazer_off, lazer_rect4);
		}
		else {
			SBDL::showTexture(lazer_on2, lazer_rect);
			SBDL::showTexture(lazer_on2, lazer_rect1);
			SBDL::showTexture(lazer_on2, lazer_rect2);
			SBDL::showTexture(lazer_off, lazer_rect3);
			SBDL::showTexture(lazer_off, lazer_rect4);
		}
		if (lazer_timer2 % 10 == 0)
			lazer_type = !lazer_type;
	}
	else if (lazer_timer2 > 200) {
		SBDL::showTexture(lazer_off, lazer_rect2);
		SBDL::showTexture(lazer_off, lazer_rect3);
		SBDL::showTexture(lazer_off, lazer_rect4);
		if (play_sound && !lazer_sound) {
			SBDL::playSound(laser_warning, 1);
			lazer_sound = true;
		}
	}
	else if (lazer_timer2 > 0) {
		if (!lazer_sound && play_sound) {
			SBDL::playSound(lazeron, 3);
			lazer_sound = true;
		}
		if (lazer_type) {
			SBDL::showTexture(lazer_on1, lazer_rect2);
			SBDL::showTexture(lazer_on1, lazer_rect3);
			SBDL::showTexture(lazer_on1, lazer_rect4);
		}
		else {
			SBDL::showTexture(lazer_on2, lazer_rect2);
			SBDL::showTexture(lazer_on2, lazer_rect3);
			SBDL::showTexture(lazer_on2, lazer_rect4);
		}
		if (lazer_timer2 % 10 == 0)
			lazer_type = !lazer_type;
	}
	if (!pause_menu && !gameover)
		lazer_timer2--;
	if (lazer_timer2 == 200 || lazer_timer2 == 0 || lazer_timer2 == 300 || lazer_timer2 == 500)
		lazer_sound = false;
	if (lazer_timer2 == 0)
		lazer = false;
	if (SBDL::hasIntersectionRect(barry_rect, lazer_rect) || SBDL::hasIntersectionRect(barry_rect, lazer_rect1) || SBDL::hasIntersectionRect(barry_rect, lazer_rect2))
		if (lazer_timer2 < 500 && lazer_timer2 > 300 && !gameover) {
			gameover = true;
			w = false;
		}
	if (SBDL::hasIntersectionRect(barry_rect, lazer_rect2) || SBDL::hasIntersectionRect(barry_rect, lazer_rect3) || SBDL::hasIntersectionRect(barry_rect, lazer_rect4))
		if (lazer_timer2 < 200 && !gameover) {
			gameover = true;
			w = false;
		}
}

void lazer3() {
	SDL_Rect lazer_rect1 = { 0, 30, window_width, 45 };
	SDL_Rect lazer_rect2 = { 0, 385, window_width, 45 };
	if (lazer_timer > 200) {
		SBDL::showTexture(lazer_off, lazer_rect1);
		SBDL::showTexture(lazer_off, lazer_rect2);
		if (play_sound && !lazer_sound) {
			SBDL::playSound(laser_warning, 1);
			lazer_sound = true;
		}
	}
	else if (lazer_timer > 0) {
		if (!lazer_sound && play_sound) {
			SBDL::playSound(lazeron, 3);
			lazer_sound = true;
		}
		if (lazer_type) {
			SBDL::showTexture(lazer_on1, lazer_rect1);
			SBDL::showTexture(lazer_on1, lazer_rect2);
		}
		else {
			SBDL::showTexture(lazer_on2, lazer_rect1);
			SBDL::showTexture(lazer_on2, lazer_rect2);
		}
		if (lazer_timer % 10 == 0)
			lazer_type = !lazer_type;
	}
	if (!pause_menu && !gameover)
		lazer_timer--;
	if (lazer_timer == 200 || lazer_timer == 0)
		lazer_sound = false;
	if (lazer_timer == 0)
		lazer = false;
	if (SBDL::hasIntersectionRect(barry_rect, lazer_rect1) || SBDL::hasIntersectionRect(barry_rect, lazer_rect2))
		if (lazer_timer < 200 && !gameover) {
			gameover = true;
			w = false;
		}
}

void missle() {
	SDL_Rect missle_rect = { x_missle, y_missle, missle1.width, missle1.height };
	if (missle_timer > 200) {
		if (y_barry > y_missle)
			y_missle += 1.5;
		else if (y_barry < y_missle)
			y_missle -= 1.5;
		SBDL::showTexture(missle_warning1, window_width - missle_warning1.width, y_missle);
	}
	else if (missle_timer > 140) {
		SBDL::showTexture(missle_warning2, window_width - missle_warning1.width, y_missle);
		if (!missle_sound) {
			SBDL::playSound(missle_warning, 1);
			missle_sound = true;
		}
	}
	else if (missle_timer > 0) {
		if (missle_type == 1) {
			SBDL::showTexture(smoke1, x_missle + missle1.width - 70, y_missle);
			SBDL::showTexture(missle1, x_missle, y_missle);
		}
		else if (missle_type == 2) {
			SBDL::showTexture(smoke2, x_missle + missle1.width - 70, y_missle);
			SBDL::showTexture(missle2, x_missle, y_missle);
		}
		else if (missle_type == 3) {
			SBDL::showTexture(smoke3, x_missle + missle1.width - 70, y_missle);
			SBDL::showTexture(missle3, x_missle, y_missle);
		}
		else if (missle_type == 4) {
			SBDL::showTexture(smoke4, x_missle + missle1.width - 70, y_missle);
			SBDL::showTexture(missle4, x_missle, y_missle);
		}
		else if (missle_type == 5) {
			SBDL::showTexture(smoke5, x_missle + missle1.width - 70, y_missle);
			SBDL::showTexture(missle5, x_missle, y_missle);
		}
		else if (missle_type == 6) {
			SBDL::showTexture(smoke6, x_missle + missle1.width - 70, y_missle);
			SBDL::showTexture(missle6, x_missle, y_missle);
		}
		if (!pause_menu && !gameover)
			x_missle -= 16;
		if (!missle_sound) {
			SBDL::playSound(missle_launch, 1);
			missle_sound = true;
		}
		if (!pause_menu && !gameover) {
			if (missle_timer % 5 == 0)
				missle_type += 1;
			if (missle_type >= 7)
				missle_type = 1;
		}
	}
	if (!pause_menu && !gameover)
		missle_timer--;
	if (missle_timer == 140 || missle_timer == 0)
		missle_sound = false;
	if (x_missle + missle1.width + smoke1.width <= 0) {
		missil = false;
		missle_sound = false;
	}
	if (SBDL::hasIntersectionRect(barry_rect, missle_rect) && missle_timer < 140 && !gameover) {
		w = false;
		gameover = true;
	}
}

void read_pattern() {
	int max_size = 0;
	i = 0;
	ifstream coin_r;
	if (coin_pattern == 0)
		coin_r.open("assets/coinPatterns/coinPattern1.txt");
	else if (coin_pattern == 1)
		coin_r.open("assets/coinPatterns/coinPattern2.txt");
	else if (coin_pattern == 2)
		coin_r.open("assets/coinPatterns/coinPattern8.txt");
	else if (coin_pattern == 3)
		coin_r.open("assets/coinPatterns/coinPattern13.txt");
	else
		coin_r.open("assets/coinPatterns/coinPattern13.txt");
	while (!coin_r.eof()) {
		coin_r >> s[i];
		if (max_size < s[i].size())
			max_size = s[i].size();
		i++;
	}
	coin_r.close();
	coin_ar = new coin*[i - 1];
	for (int j = 0; j < i - 1; j++) {
		x_coin = window_width;
		y_coin += 25;
		coin_ar[j] = new coin[max_size];
		for (int k = 0; k < s[j].size(); k++) {
			if (s[j][k] == '0')
				coin_ar[j][k].type = 1;
			else /*if (s[j][k] == '1')*/
				coin_ar[j][k].type = 4;
			coin_ar[j][k].x = x_coin;
			coin_ar[j][k].y = y_coin;
			x_coin += 25;
		}
	}
}

void speed_token() {
	SDL_Rect speed_token_rect = { x_speed_token, y_speed_token + i_speed_token * v_bg / 5, 50, 50 };
	if (speed_token_type == 1)
		SBDL::showTexture(speed_token1, speed_token_rect);
	else if (speed_token_type == 2)
		SBDL::showTexture(speed_token2, speed_token_rect);
	else if (speed_token_type == 3)
		SBDL::showTexture(speed_token3, speed_token_rect);
	else if (speed_token_type == 4)
		SBDL::showTexture(speed_token4, speed_token_rect);
	if (speed_token_timer == 0 && !pause_menu && !gameover) {
		speed_token_type++;
		speed_token_timer = 15;
	}
	if (speed_token_type == 5)
		speed_token_type = 1;
	if (!pause_menu && !gameover)
		speed_token_timer--;
	if (-100 >= i_speed_token * v_bg / 5) {
		down = true;
		up = false;
	}
	else if (100 <= i_speed_token * v_bg / 5) {
		up = true;
		down = false;
	}
	if (!pause_menu && !gameover) {
		if (up)
			i_speed_token--;
		else if (down)
			i_speed_token++;
		x_speed_token -= v_bg / 5;
	}
	if (x_speed_token <= 0)
		speedToken = false;
	if (SBDL::hasIntersectionRect(barry_rect, speed_token_rect)) {
		boost = true;
		speed_token_timer = 150;
		SBDL::playSound(boost_sound, 1);
	}
}

void gameover_page() {
	read_high();
	if (distanc > best_score) {
		h9 = h8;
		h8 = h7;
		h7 = h6;
		h6 = h5;
		h5 = h4;
		h4 = h3;
		h3 = h2;
		h2 = h1;
		h1 = best_score;
		best_score = distanc;
		w = true;
	}
	write_high();
	//SBDL::stopAllSounds();
	SDL_Rect rec = { 0, 0, window_width, window_height };
	SDL_Rect health_rect = { window_width / 2 + 55, 155, 90, 90 };
	SDL_Rect back_rect = { window_width / 2 + 20, 350, 150, 50 };
	SDL_Rect retry_rect = { window_width / 2 - 170, 350, 150, 50 };
	SBDL::drawRectangle(rec, 0, 0, 0, 150);
	SBDL::showTexture(gameover_title, 0, -3);
	SBDL::showTexture(health, health_rect);
	SBDL::freeTexture(distance_texture);
	SBDL::freeTexture(best_texture);
	SBDL::freeTexture(coin_texture);
	distance_texture = SBDL::createFontTexture(jetpackia30, "score: " + to_string(distanc) + " m", 171, 171, 171);
	best_texture = SBDL::createFontTexture(jetpackia20, "best: " + to_string(best_score), 171, 171, 171);
	coin_texture = SBDL::createFontTexture(jetpackia20, to_string(coins), 254, 254, 60);
	SBDL::showTexture(distance_texture, window_width / 2 - distance_texture.width - 55, 150);
	SBDL::showTexture(best_texture, window_width / 2 - distance_texture.width - 55, 152 + distance_texture.height);
	SBDL::showTexture(coin1, window_width / 2 - distance_texture.width - 55, 156 + best_texture.height + distance_texture.height);
	SBDL::showTexture(coin_texture, window_width / 2 - distance_texture.width + 15 + coin1.width - 55, 154 + best_texture.height + distance_texture.height);
	SBDL::showTexture(back, back_rect);
	SBDL::showTexture(retry, retry_rect);
	if (SBDL::mouseInRect(retry_rect) && SBDL::Mouse.clicked()) {
		w = false;
		gameover = false;
		y_barry = window_height - barry2.height - 20;
		barry_timer = 10;
		distanc = 0;
		bg_type = 1;
		bg_x = 0;
		speedToken = false;
		lazer = false;
		missil = false;
		sh_coin = false;
		show_zapper = false;
	}
	if (SBDL::mouseInRect(back_rect) && SBDL::Mouse.clicked()) {
		w = false;
		gameover = false;
		y_barry = window_height - barry2.height - 20;
		barry_timer = 10;
		distanc = 0;
		bg_type = 1;
		bg_x = 0;
		start_menu = true;
		SBDL::stopMusic();
		music_is_playing = false;
		speedToken = false;
		lazer = false;
		missil = false;
		sh_coin = false;
		show_zapper = false;
	}
	if (SBDL::mouseInRect(health_rect) && SBDL::Mouse.clicked() && coins >= 500) {
		w = false;
		SBDL::playSound(health_sound, 1);
		coins -= 500;
		speedToken = false;
		lazer = false;
		missil = false;
		sh_coin = false;
		gameover = false;
		show_zapper = false;
	}
	ofstream coin_w;
	coin_w.open("coins.txt");
	coin_w << coins << endl;
	coin_w.close();
}

void zapper() {
	SDL_Rect z1_rect;
	SDL_Rect z2_rect;
	SDL_Rect z3_rect;
	if (z1_t == 0) {
		z1_rect = { x_z1, y_z1, 200, 60 };
		if (SBDL::hasIntersectionRect(barry_rect, z1_rect) && !speedToken && !gameover) {
			gameover = true;
			if (play_sound)
				SBDL::playSound(zapper_sound, 1);
		}
		if (z1_type == 0)
			SBDL::showTexture(zapper_h1, z1_rect);
		else if (z1_type == 1)
			SBDL::showTexture(zapper_h2, z1_rect);
		else if (z1_type == 2)
			SBDL::showTexture(zapper_h3, z1_rect);
		else /*if (z1_type == 3)*/
			SBDL::showTexture(zapper_h4, z1_rect);
	}
	else if (z1_t == 1) {
		z1_rect = { x_z1, y_z1, 60, 200 };
		if (SBDL::hasIntersectionRect(barry_rect, z1_rect) && !speedToken && !gameover) {
			gameover = true;
			if (play_sound)
				SBDL::playSound(zapper_sound, 1);
		}
		if (z1_type == 0)
			SBDL::showTexture(zapper_v1, z1_rect);
		else if (z1_type == 1)
			SBDL::showTexture(zapper_v2, z1_rect);
		else if (z1_type == 2)
			SBDL::showTexture(zapper_v3, z1_rect);
		else /*if (z1_type == 3)*/
			SBDL::showTexture(zapper_v4, z1_rect);
	}
	else /*if (z1_t == 2)*/ {
		z1_rect = { x_z1, y_z1, 150, 150 };
		line1 = { x_z1 + 150, y_z1, x_z1, y_z1 + 150 };
		if (SDL_IntersectRectAndLine(&barry_rect, &line1.x1, &line1.y1, &line1.x2, &line1.y2) && !speedToken && !gameover) {
			gameover = true;
			if (play_sound)
				SBDL::playSound(zapper_sound, 1);
		}
		if (z1_type == 0)
			SBDL::showTexture(zapper_d1, z1_rect);
		else if (z1_type == 1)
			SBDL::showTexture(zapper_d2, z1_rect);
		else if (z1_type == 2)
			SBDL::showTexture(zapper_d3, z1_rect);
		else /*if (z1_type == 3)*/
			SBDL::showTexture(zapper_d4, z1_rect);
	}
	if (z2_t == 0) {
		z2_rect = { x_z2, y_z2, 200, 60 };
		if (SBDL::hasIntersectionRect(barry_rect, z2_rect) && !speedToken && !gameover) {
			gameover = true;
			if (play_sound)
				SBDL::playSound(zapper_sound, 1);
		}
		if (z2_type == 0)
			SBDL::showTexture(zapper_h1, z2_rect);
		else if (z2_type == 1)
			SBDL::showTexture(zapper_h2, z2_rect);
		else if (z2_type == 2)
			SBDL::showTexture(zapper_h3, z2_rect);
		else /*if (z2_type == 3)*/
			SBDL::showTexture(zapper_h4, z2_rect);
	}
	else if (z2_t == 1) {
		z2_rect = { x_z2, y_z2, 60, 200 };
		if (SBDL::hasIntersectionRect(barry_rect, z2_rect) && !speedToken && !gameover) {
			gameover = true;
			if (play_sound)
				SBDL::playSound(zapper_sound, 1);
		}
		if (z2_type == 0)
			SBDL::showTexture(zapper_v1, z2_rect);
		else if (z2_type == 1)
			SBDL::showTexture(zapper_v2, z2_rect);
		else if (z2_type == 2)
			SBDL::showTexture(zapper_v3, z2_rect);
		else /*if (z2_type == 3)*/
			SBDL::showTexture(zapper_v4, z2_rect);
	}
	else /*if (z2_t == 2)*/ {
		z2_rect = { x_z2, y_z2, 150, 150 };
		line2 = { x_z2 + 150, y_z2, x_z2, y_z2 + 150 };
		if (SDL_IntersectRectAndLine(&barry_rect, &line2.x1, &line2.y1, &line2.x2, &line2.y2) && !speedToken && !gameover) {
			gameover = true;
			if (play_sound)
				SBDL::playSound(zapper_sound, 1);
		}
		if (z2_type == 0)
			SBDL::showTexture(zapper_d1, z2_rect);
		else if (z2_type == 1)
			SBDL::showTexture(zapper_d2, z2_rect);
		else if (z2_type == 2)
			SBDL::showTexture(zapper_d3, z2_rect);
		else /*if (z2_type == 3)*/
			SBDL::showTexture(zapper_d4, z2_rect);
	}
	if (z3_t == 0) {
		z3_rect = { x_z3, y_z3, 200, 60 };
		if (SBDL::hasIntersectionRect(barry_rect, z3_rect) && !speedToken && !gameover) {
			gameover = true;
			if (play_sound)
				SBDL::playSound(zapper_sound, 1);
		}
		if (z3_type == 0)
			SBDL::showTexture(zapper_h1, z3_rect);
		else if (z3_type == 1)
			SBDL::showTexture(zapper_h2, z3_rect);
		else if (z3_type == 2)
			SBDL::showTexture(zapper_h3, z3_rect);
		else /*if (z3_type == 3)*/
			SBDL::showTexture(zapper_h4, z3_rect);
	}
	else if (z3_t == 1) {
		z3_rect = { x_z3, y_z3, 60, 200 };
		if (SBDL::hasIntersectionRect(barry_rect, z3_rect) && !speedToken && !gameover) {
			gameover = true;
			if (play_sound)
				SBDL::playSound(zapper_sound, 1);
		}
		if (z3_type == 0)
			SBDL::showTexture(zapper_v1, z3_rect);
		else if (z3_type == 1)
			SBDL::showTexture(zapper_v2, z3_rect);
		else if (z3_type == 2)
			SBDL::showTexture(zapper_v3, z3_rect);
		else /*if (z3_type == 3)*/
			SBDL::showTexture(zapper_v4, z3_rect);
	}
	else/* if (z3_t == 2)*/ {
		z3_rect = { x_z3, y_z3, 150, 150 };
		line3 = { x_z3 + 150, y_z3, x_z3, y_z3 + 150 };
		if (SDL_IntersectRectAndLine(&barry_rect, &line3.x1, &line3.y1, &line3.x2, &line3.y2) && !speedToken && !gameover) {
			gameover = true;
			if (play_sound)
				SBDL::playSound(zapper_sound, 1);
		}
		if (z3_type == 0)
			SBDL::showTexture(zapper_d1, z3_rect);
		else if (z3_type == 1)
			SBDL::showTexture(zapper_d2, z3_rect);
		else if (z3_type == 2)
			SBDL::showTexture(zapper_d3, z3_rect);
		else /*if (z3_type == 3)*/
			SBDL::showTexture(zapper_d4, z3_rect);
	}
	if (!pause_menu && !gameover) {
		x_z1 -= v_bg;
		x_z2 -= v_bg;
		x_z3 -= v_bg;
		zapper_timer--;
	}
	if (zapper_timer == 0) {
		zapper_timer = 10;
		z1_type++;
		z2_type++;
		z3_type++;
		if (z1_type >= 4)
			z1_type = 0;
		if (z2_type >= 4)
			z2_type = 0;
		if (z3_type >= 4)
			z3_type = 0;
	}
	if (x_z3 + 200 < 0)
		show_zapper = false;
}

void highscores_page() {
	read_high();
	SDL_Rect rec = { 0, 0, window_width, window_height };
	SDL_Rect close_rect = { window_width / 2 + setting_bg.width / 2 - 60, 20, 45, 45 };
	SBDL::drawRectangle(rec, 0, 0, 0, 200);
	SBDL::showTexture(high_bg, window_width / 2 - high_bg.width / 2, 5);
	if (SBDL::mouseInRect(close_rect) && SBDL::Mouse.clicked())
		highscore = false;
	SBDL::freeTexture(h1_t);
	SBDL::freeTexture(h2_t);
	SBDL::freeTexture(h3_t);
	SBDL::freeTexture(h4_t);
	SBDL::freeTexture(h5_t);
	SBDL::freeTexture(h6_t);
	SBDL::freeTexture(h7_t);
	SBDL::freeTexture(h8_t);
	SBDL::freeTexture(h9_t);
	SBDL::freeTexture(h10_t);
	h1_t = SBDL::createFontTexture(jetpackia30, to_string(best_score), 171, 171, 171);
	h2_t = SBDL::createFontTexture(jetpackia30, to_string(h1), 171, 171, 171);
	h3_t = SBDL::createFontTexture(jetpackia30, to_string(h2), 171, 171, 171);
	h4_t = SBDL::createFontTexture(jetpackia30, to_string(h3), 171, 171, 171);
	h5_t = SBDL::createFontTexture(jetpackia30, to_string(h4), 171, 171, 171);
	h6_t = SBDL::createFontTexture(jetpackia30, to_string(h5), 171, 171, 171);
	h7_t = SBDL::createFontTexture(jetpackia30, to_string(h6), 171, 171, 171);
	h8_t = SBDL::createFontTexture(jetpackia30, to_string(h7), 171, 171, 171);
	h9_t = SBDL::createFontTexture(jetpackia30, to_string(h8), 171, 171, 171);
	h10_t = SBDL::createFontTexture(jetpackia30, to_string(h9), 171, 171, 171);
	SBDL::showTexture(h1_t, window_width / 2 - h1_t.width / 2, 85);
	SBDL::showTexture(h2_t, window_width / 2 - h2_t.width / 2, 121);
	SBDL::showTexture(h3_t, window_width / 2 - h3_t.width / 2, 157);
	SBDL::showTexture(h4_t, window_width / 2 - h4_t.width / 2, 193);
	SBDL::showTexture(h5_t, window_width / 2 - h5_t.width / 2, 229);
	SBDL::showTexture(h6_t, window_width / 2 - h6_t.width / 2, 265);
	SBDL::showTexture(h7_t, window_width / 2 - h7_t.width / 2, 301);
	SBDL::showTexture(h8_t, window_width / 2 - h8_t.width / 2, 337);
	SBDL::showTexture(h9_t, window_width / 2 - h9_t.width / 2, 373);
	SBDL::showTexture(h10_t, window_width / 2 - h10_t.width / 2, 409);

}

void read_high() {
	ifstream high_r;
	high_r.open("best_scores.txt");
	high_r >> best_score;
	high_r >> h1;
	high_r >> h2;
	high_r >> h3;
	high_r >> h4;
	high_r >> h5;
	high_r >> h6;
	high_r >> h7;
	high_r >> h8;
	high_r >> h9;
	high_r.close();
}

void write_high() {
	high_w.open("best_scores.txt");
	high_w.clear();
	high_w << best_score << endl;
	if (distanc > h1 && !w) {
		high_w << distanc << endl;
		w = true;
		h9 = h8;
		h8 = h7;
		h7 = h6;
		h6 = h5;
		h5 = h4;
		h4 = h3;
		h3 = h2;
		h2 = h1;
	}
	else
		high_w << h1 << endl;
	if (distanc > h2 && !w) {
		high_w << distanc << endl;
		w = true;
		h9 = h8;
		h8 = h7;
		h7 = h6;
		h6 = h5;
		h5 = h4;
		h4 = h3;
		h3 = h2;
	}
	else
		high_w << h2 << endl;
	if (distanc > h3 && !w) {
		high_w << distanc << endl;
		w = true;
		h9 = h8;
		h8 = h7;
		h7 = h6;
		h6 = h5;
		h5 = h4;
		h4 = h3;
	}
	else
		high_w << h3 << endl;
	if (distanc > h4 && !w) {
		high_w << distanc << endl;
		w = true;
		h9 = h8;
		h8 = h7;
		h7 = h6;
		h6 = h5;
		h5 = h4;
	}
	else
		high_w << h4 << endl;
	if (distanc > h5 && !w) {
		high_w << distanc << endl;
		w = true;
		h9 = h8;
		h8 = h7;
		h7 = h6;
		h6 = h5;
	}
	else
		high_w << h5 << endl;
	if (distanc > h6 && !w) {
		high_w << distanc << endl;
		w = true;
		h9 = h8;
		h8 = h7;
		h7 = h6;
	}
	else
		high_w << h6 << endl;
	if (distanc > h7 && !w) {
		high_w << distanc << endl;
		w = true;
		h9 = h8;
		h8 = h7;
	}
	else
		high_w << h7 << endl;
	if (distanc > h8 && !w) {
		high_w << distanc << endl;
		w = true;
		h9 = h8;
	}
	else
		high_w << h8 << endl;
	if (distanc > h9 && !w)
		high_w << distanc << endl;
	else if (!w)
		high_w << h9 << endl;
	high_w.close();
}
