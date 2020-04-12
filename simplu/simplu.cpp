
#include<windows.h>
#include<stdio.h>
#include<stddef.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../inc/universe.h"

char*text_format() {
	return R"(
mv=createTextField('%s',getNextHighestDepth(),%u,%u,%u,%u);
mv.background=true;
mv.backgroundColor=0x00ffff;
mv.border=true;
mv.borderColor=0xff0000;
fmt=new TextFormat();fmt.size=%u;fmt.align='%s';
mv.setNewTextFormat(fmt);
)";//setNewTextFormat ca .text= este dupa, daca era fara New selecta tot textul existent(pentru o parte de text sunt argumente)si aplica formatul
}
char* top_text(int w,int h) {
	sprintf(buf, text_format(), "top",0, 0, w, h,h-10, "center"); return buf;
}
//
char*starter() { return "_root.start_scenario();"; }
char*ender() { return "_root.restart_scenario();"; }
char*enter1() {
	//chiar daca area.removemovie tot mai persista enter, chiar daca game.area.ascultator e undefined
	return "listener=new Object();listener.onKeyDown=function(){";
}
char*enter2() {return "if(Key.getCode()==Key.ENTER){"; }
char*enter3() {return "}}Key.addListener(listener);";}

#ifdef __cplusplus
extern "C" {
#endif
	//SharedObject: keys levelIsDone listSort
	int _start() {
		char nume[3*MAX_PATH];//ca doar este si fisierul cu acelasi nume + nume_ecran + ce mai este
		FILE* fp = fopen("ep.txt", "rb");
		if (fp == NULL) { printf("\nep.txt"); return 0; }
		fseek(fp, 0, SEEK_END); int sz = ftell(fp); fseek(fp, 0, SEEK_SET);
		fread(nume, 1, sz, fp); nume[sz] = 0; fclose(fp);
		char*a = strpbrk(nume, ",");
		char*nume_ecran = strpbrk(a, ";");
		a[0] = 0; a++;
		if (nume_ecran == NULL)nume_ecran = nume;
		else{
			nume_ecran[0] = 0; nume_ecran++;
			char*aux = a; a = nume_ecran; nume_ecran = aux;
		}
		bool circle = a[0] != '0';
		a++; int fuel = a[0]-'0';
		a++; int rotation; buf[0] = '0'; buf[1] = 'x'; buf[2] = a[0]; buf[3] = 0;sscanf(buf, "%x", &rotation);
		episoade_pack*ep = episoade[find_ep(nume)];
		bool are_chei = ep->episod_chei != NULL;
		//a++; bool keys = a[0] != '0';
		a++; bool counterbar = a[0] != '0';
		a++; bool automatics = a[0] != '0';
		a++; bool are_vars = a[0] != '0';
		printf("\n%s %s cerc=%u fuel=%u rotire=%u counterbar=%u automatics=%u are_vars=%u        key=%u", nume,nume_ecran,circle, fuel, rotation,counterbar, automatics, are_vars,are_chei);

		fp = fopen("test", "rb");
		bool doar_numele_si_are_vars = fp != NULL;
		
		if (doar_numele_si_are_vars)fclose(fp);
		else{
		sprintf(buf, "C:/Desktop/universe/swf_backup/multi/_%s.swf", nume);
		swf_new(buf, width_nr, height, 0x000000, fps);

		int defFont = swf_font_basic("_sans");//despre font_basic: gnash observa o neregula, dar flex nu zice nimic rau
		//@SWFType(BasicType.UI8) este LANGCODE: https://github.com/DV8FromTheWorld/0x40-Hues-Editor/blob/master/library/ffdec/src/com/jpexs/decompiler/flash/types/LANGCODE.java
		//2 idul 1 flag 1 langcode 1+5 _sans si inca doua num glyphs

		char* bar_path = "C:/Desktop/universe/img/root/bar.dbl";
		dbl = swf_dbl(bar_path);
		swf_exports_add(dbl, "bar_texture");

		char* bar_back_path = "C:/Desktop/universe/img/0/back.dbl";
		dbl = swf_dbl(bar_back_path);
		swf_exports_add(dbl, "bar_back");

		presprite=swf_sprite_new();
		actionf_sprite(presprite, buf, R"(
		var m=flash.display.BitmapData.loadBitmap('bar_texture');
		beginBitmapFill(m,new flash.geom.Matrix());
		lineTo(%u,0);lineTo(%u,%u);lineTo(0,%u);
		endFill();
		attachMovie('bar_back_b','bar_back',getNextHighestDepth())
		function delayedFunction(){bar_back_info._visible=true;bar_back_time=undefined;setTimeout(delayedFunction2,3000);}
	//la tinut apasat press si rollout uita de rollout, onReleaseOutside e inconsistenta
		function delayedFunction2(){if(bar_back_time==undefined)bar_back_info._visible=false;}
		var w=200;
		createTextField('bar_back_info',getNextHighestDepth(),0-w,0,w,40);
		bar_back_info.background=true;
		bar_back_info.backgroundColor=0x00ffff;
		bar_back_info.border=true;
		bar_back_info.borderColor=0xff00ff;
		bar_back_info.text='Back to home';
		//size 12 e default
		//nu exista TextField.removeMovieClip()
		var fmt=new TextFormat();fmt.size=30;fmt.align='center';bar_back_info.setTextFormat(fmt);
		//}
		bar_back_info._visible=false;
		var bar_back_time;
		bar_back.onRollOver=function(){
			if(bar_back_info._visible==false){
				if(bar_back_time==undefined){
					bar_back_time=setTimeout(delayedFunction,1000);
				}
			}
		}
		bar_back.onRollOut=function(){
			clearTimeout(bar_back_time);
			//trebuie si asta, e testat
			bar_back_time=undefined;
			//
			bar_back_info._visible=false;}
		)", bar_dimensiune, bar_dimensiune, height, height);
		sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bar_line");

		char*counter_related = "";
		presprite = swf_sprite_new();
		actionf_sprite(presprite, buf, R"(
		var m=flash.display.BitmapData.loadBitmap('bar_back');
		var w=m.width;var h=m.height;
		beginBitmapFill(m,new flash.geom.Matrix(),false);
		lineTo(w,0);lineTo(w,h);lineTo(0,h);
		endFill();
		//
		onPress=function(){
			%s
			_root.restart_scen();
		}
		_root.at_start();
		)", counterbar ? "_root.bar.counterBar.removeMovieClip();":"");
		sprite = swf_sprite_done(presprite); swf_exports_add(sprite, "bar_back_b");

		int wd = 504; int rand1_h = 50;
		int rand2_h = 200; int key_nr;
		if (are_chei) {
			//asta este in swf init_episod(nume);
			key_nr = get_ep_nr(ep->episod_chei);
			if (key_nr == 9)rand2_h -= 20;
		}
		int bt_w = 150; int bt_x = (wd - bt_w) / 2; int bt_h = 50;
		int xul = (width - wd) / 2; int hul=height - rand1_h - bt_h;
		int yul = hul / 2;

		action("var key_default_code=new Array();var key_names=new Array();var textfield_keysets=new Array();var textfield_keyset=-1");
		presprite = swf_sprite_new();
		ButtonData bd = { 0x21fe21ff,10,0xee1122ff,0x2121feff,10,0x883344ff,0xfe2121ff,10,0x225566ff,24,24,"Play",defFont,20,-5,0xff,starter()};
		int bt = swf_button(bt_w, bt_h, &bd);
		//se poate action si placeobject depth in acelasi loc dar action vor fi intotdeauna peste placeobjectdepth
		action_sprite(presprite, top_text(wd,rand1_h));
		actionf_sprite(presprite, buf, "top.text='%s'", nume_ecran);
		actionf_sprite(presprite, buf, text_format(), "desc",0, rand1_h,wd, rand2_h, rand1_h - 30, "justify", ep->description);
		actionf_sprite(presprite, buf, "desc.text='%s';desc.wordWrap=true", ep->description);
		int yoff = rand2_h; int w_nr_2 = wd / 2; int rand3_h = rand1_h - 20; int f3 = rand3_h - 10;
		int y = rand1_h + yoff;
		char nm[12];//1000 chei
		if (are_chei) {
			for (int i = 0; i < key_nr; i++) {
				//align, The default value is null, which indicates that the property is undefined. 
				sprintf(nm, "k_left%u", i); actionf_sprite(presprite, buf, text_format(), nm, 0, y, w_nr_2, rand3_h, f3, "left");
				actionf_sprite(presprite, buf, "%s.text='%s'", nm, ep->episod_chei[i].key_alt);
				sprintf(nm, "k_right%u", i); actionf_sprite(presprite, buf, text_format(), nm, w_nr_2, y, w_nr_2, rand3_h, f3, "left");
				actionf_sprite(presprite, buf, "%s.type='input';%s.textColor=0x0000ff;%s.restrict=''", nm, nm,nm);
				//If the value of the restrict property is an empty string, you cannot enter any character
				//This only restricts user interaction; a script may put any text into the text field.
				actionf_sprite(presprite, buf, R"(
				_root.textfield_keysets[%u]=%s;
				%s.onSetFocus=function(){
					%s.text='';_root.textfield_keyset=%u;
				}
				%s.onKillFocus=function(){
					if(%s.text=='')_root.key_text(%s,%u);
					_root.textfield_keyset=-1;
				}
				_root.key_text(%s,%u);
				)",i,nm,nm,nm,i,nm,nm,nm,i,nm,i);
				yoff += rand3_h;
				//
				actionf(buf, "key_default_code[%u]=%s;key_names[%u]='%s'", i, get_def_code(ep->episod_chei[i].key->defaultKey), i, ep->episod_chei[i].key->name);
			}
		}
		else {
			sprintf(nm, "k_left"); actionf_sprite(presprite, buf, text_format(), nm, 0, y, w_nr_2, rand3_h, f3, "left");
			actionf_sprite(presprite, buf, "%s.text='%s'", nm, mouse_disp_name);
			sprintf(nm, "k_right"); actionf_sprite(presprite, buf, text_format(), nm, w_nr_2, y, w_nr_2, rand3_h, f3, "left");
			actionf_sprite(presprite, buf, "%s.text='%s'", nm, ep->mousePress);
			yoff += rand3_h;
		}

		int w_nr_3 = wd / 3;
		int m_y = rand1_h + yoff;
		actionf_sprite(presprite, buf, text_format(), "mus", 0, m_y, w_nr_3, rand3_h, f3, "left"); action_sprite(presprite, "mus.text='Music'"); yoff += rand3_h;
		//
		int premuson = swf_sprite_new();
		actionf_sprite(premuson, buf, text_format(), "mus_on", 0, 0, w_nr_3, rand3_h, f3, "left"); action_sprite(premuson, "mus_on.text='On'");
		action_sprite(premuson, R"(
		var so_m=SharedObject.getLocal('music');
		onPress=function(){_parent.checking._x=_x;
			if(so_m.data['on']==false)_root.music.start();
			so_m.data['on']=true;so_m.flush();}
		)");
		int muson = swf_sprite_done(premuson);
		swf_sprite_placeobject_coords(presprite, muson, 0, w_nr_3, m_y);
		//
		int premusoff = swf_sprite_new();
		actionf_sprite(premusoff, buf, text_format(), "mus_off", 0, 0, w_nr_3, rand3_h, f3, "left"); action_sprite(premusoff, "mus_off.text='Off'");
		action_sprite(premusoff, R"(
		var so_m=SharedObject.getLocal('music');
		onPress=function(){_parent.checking._x=_x;_root.music.stop();
			so_m.data['on']=false;so_m.flush();}
		)");
		int musoff = swf_sprite_done(premusoff);
		swf_sprite_placeobject_coords(presprite, musoff, 1, w_nr_3 + w_nr_3, m_y);
		//
		int prechecking = swf_sprite_new();
		actionf_sprite(prechecking, buf, "lineStyle(1,0xff0000);moveTo(%u,0);lineTo(%u,%u);lineTo(%u,%u)", w_nr_3, w_nr_3 - 20, rand3_h, w_nr_3 - 25, rand3_h - 5);
		int checking = swf_sprite_done(prechecking); swf_exports_add(checking, "checking");
		actionf_sprite(presprite,buf, "attachMovie('checking','checking',getNextHighestDepth());checking._y=%u;checking._x=%u",m_y, w_nr_3);
		actionf_sprite(presprite, buf, "var so_m=SharedObject.getLocal('music');if(so_m.data['on']==false)checking._x+=%u", w_nr_3);
		
		int yul_start = (hul - yoff) / 2;
		swf_sprite_placeobject_coords(presprite, bt, 2, bt_x,rand1_h+ yoff);
		sprite = swf_sprite_done(presprite); swf_exports_add(sprite, "list_view");
		
		presprite = swf_sprite_new();
		int bt2 = swf_button_last("OK", ender());
		action_sprite(presprite, top_text(wd, rand1_h));
		action_sprite(presprite, "var end_time=new Date();var s=int((end_time.valueOf()-_root.start_time.valueOf())/1000);top.text='Time Score: '+s");
		action_sprite(presprite, "var kongregate=_global.kongregate;kongregate.services.connect();kongregate.stats.submit('Highscore',s)");
		if (counterbar)action_sprite(presprite, "_root.bar.counterBar.removeMovieClip()");
		swf_sprite_placeobject_coords(presprite, bt2, 1, bt_x, rand1_h);
		sprite = swf_sprite_done(presprite); swf_exports_add(sprite, "scenario_done");

		if (counterbar) {
			//counterBar
			presprite = swf_sprite_new();
			actionf_sprite(presprite, buf, "var bar_h=%u", bar_h);
			action_sprite(presprite, "var bar_sz=bar_h-_y");
			actionf_sprite(presprite, buf, "var bar_w=%u", bar_w);
			action_sprite(presprite, R"(
		moveTo(bar_w,bar_h);
        function draw_progressBar(){
            var current_pos=bar_sz*barNr/barMax;
			clear();moveTo(0,current_pos);
			var red=int(255*barNr/barMax);var blue=int(255*(barMax-barNr)/barMax);
            beginFill((red*0x10000)|blue);lineTo(bar_w,current_pos);lineTo(bar_w,bar_h);lineTo(0,bar_h);
            endFill();
        }
        draw_progressBar();
    )");
			sprite = swf_sprite_done(presprite); swf_exports_add(sprite, "counterBar");
			action(R"(
        function counterBar_init(max_pos){
            var mc=bar.attachMovie('counterBar','counterBar',bar.getNextHighestDepth());
            mc._y=bar.bar_back._height;mc['barNr']=0;mc['barMax']=max_pos;
        }
		function counterBar_step(){
			counterBar_jump(1);
        }
		function counterBar_jump(a){
            bar.counterBar.barNr+=a;
            if((bar.counterBar.barNr<bar.counterBar.barMax)==false)end_scenario();
			else bar.counterBar.draw_progressBar();
        })");
		}

		//com
		shared_write_ex(circle, fuel, rotation, true, are_chei);

		char game_clear[100]; sprintf(game_clear, "%sgame.area.removeMovieClip();", automatics ? "automatics_func();" : "");
		actionf(buf, R"(
		var start_time;var listener;
		function to_action(){Key.removeListener(listener);game.area.removeMovieClip();}
		function start_scenario(){
			//se poate si fara dar decat _x=0 _y=0 mai bine asa
			to_action();
			game.createEmptyMovieClip('area',game.getNextHighestDepth());
			//
			game.area.loadMovie('%s.swf');
			start_time=new Date();
		}
		function end_scenario(){
			%s
			var mc=game.attachMovie('scenario_done','area',game.getNextHighestDepth());
			mc._x=%u;mc._y=%u;
			%s%s%s%s
		}
		)", nume, game_clear,xul,yul,enter1(),enter2(),ender(),enter3());
		actionf(buf, R"(
		attachMovie('shared_level','shared_level',getNextHighestDepth());
		shared_level.onEnterFrame=function(){
			this.onEnterFrame=undefined;
//cand deja are functile
			this.space_env(_root,'env',_root.getNextHighestDepth());
//game e dupa care area ca la cele cu background care depaseste ecranul sa fie sub bar_line si game e la depth , doar game.area loadmovie si remove
			createEmptyMovieClip('game',getNextHighestDepth());
			attachMovie('bar_line','bar',getNextHighestDepth());bar._x=%u;
		}))",width);
		actionf(buf,R"(
		var music=new Sound();
        music.onLoad=function(success){
			var so_m=SharedObject.getLocal('music');
			if(so_m.data['on']!=false)
				this.start();
		}
        music.onSoundComplete=function(){this.start();}
		music.loadSound('%s.mp3',false);
		)", nume);
		char* cheile = "";
		if(are_chei)cheile = R"(
			if(textfield_keyset!=-1){
				var tf=textfield_keysets[textfield_keyset];
				var key_id=key_names[textfield_keyset];
				var code=Key.getCode();
                //
				shared_level[key_id]=code;
                shared_level.so_keys.data[key_id]=code;
                shared_level.so_keys.flush();
                //
				tf.text=keycode_to_string(code);
				//daca era varianta de killFocus poate era mai bine
				//If null or undefined is passed, the current focus is removed. 
				Selection.setFocus(null);
			}else )";
		actionf(buf,"function restart_scenario(){to_action();at_start();}function restart_scen(){%sat_start();}", game_clear);
		actionf(buf, R"(
		function at_start(){
//peste game.area ca acolo vine loadmovie si sterge tot
			var mc=game.attachMovie('list_view','area',game.getNextHighestDepth());
			mc._x=%u;mc._y=%u;
			%s%s%s%s%s
		}
		)", xul, yul_start, enter1(), cheile, enter2(),starter(), enter3());
		action(R"(
		function keycode_to_string(keycode){
            if(keycode==Key.BACKSPACE)return 'BACKSPACE';
            if(keycode==Key.CAPSLOCK)return 'CAPSLOCK';
            if(keycode==Key.CONTROL)return 'CONTROL';
            if(keycode==Key.DELETEKEY)return 'DELETEKEY';
            if(keycode==Key.DOWN)return 'DOWN';
            if(keycode==Key.END)return 'END';
            if(keycode==Key.ENTER)return 'ENTER';
            if(keycode==Key.ESCAPE)return 'ESCAPE';
            if(keycode==Key.HOME)return 'HOME';
            if(keycode==Key.INSERT)return 'INSERT';
            if(keycode==Key.LEFT)return 'LEFT';
            if(keycode==Key.PGDN)return 'PGDN';
            if(keycode==Key.PGUP)return 'PGUP';
            if(keycode==Key.RIGHT)return 'RIGHT';
            if(keycode==Key.SHIFT)return 'SHIFT';
            if(keycode==Key.SPACE)return 'SPACE';
            if(keycode==Key.TAB)return 'TAB';
            if(keycode==Key.UP)return 'UP';
            return String.fromCharCode(keycode);
        }
		function key_text(mc,i){
			var a;
			if(shared_level[key_names[i]]==undefined)a=key_default_code[i];
			else a=shared_level[key_names[i]];
			mc.text=keycode_to_string(a);
		}
		)");

		swf_exports_done();
		//swf_showframe();//trebuie dupa ExportAssers
		swf_done();
		}

		FILE* f = fopen("nume.txt", "wb");
		fwrite(nume, 1, strlen(nume), f);
		fclose(f);

		char* vrs = "are_vars";
		if (are_vars) {
			f = fopen(vrs, "wb");fclose(f);
		}else
			//remove nu trow nimic daca nu e
			remove(vrs);

		printf("\ne done");
		return 0;
	}
#ifdef __cplusplus
}
#endif