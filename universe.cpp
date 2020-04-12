
#include<windows.h>
#include<stdio.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif
//SharedObject: keys levelIsDone listSort
int _start(){
    swf_new(/*rooted_swf_path("universe")*/"test/universe.swf",width_nr, height,0x000000,fps);

    int defFont=swf_font_basic("_sans");

    //bar

    char* bar_path="C:/Desktop/universe/img/root/bar.dbl";
    dbl=swf_dbl(bar_path);
    swf_exports_add(dbl,"bar_texture");
    //root
    int button_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/root/root.dbl",button_sz);
    swf_exports_add(dbl,"bar_home");
    //sound
    dbl=swf_dbl("C:/Desktop/universe/img/root/sound.dbl");
    swf_exports_add(dbl,"bar_sound");
    dbl=swf_dbl("C:/Desktop/universe/img/root/sound_off.dbl");
    swf_exports_add(dbl,"bar_sound_off");
    //keyboard
    dbl=swf_dbl("C:/Desktop/universe/img/root/keyboard.dbl");
    swf_exports_add(dbl,"bar_keyboard");
    //
    int button_lat=button_sz[1];
    int button_szAndInter=button_lat+5;
    int bar_coord=0;
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var bar_w=%u;var bar_h=%u",bar_w,bar_h);
    actionf_sprite(presprite,buf,"var button_lat=%u",button_lat);
    action_sprite(presprite,R"(
        var bmp;
        var mx=new flash.geom.Matrix();
        function draw_bmp(mc,img,wd,hg){
            bmp=flash.display.BitmapData.loadBitmap(img);
            fill_mc(mc,bmp,wd,hg);
        }
        function fill_mc(mc,bmp,wd,hg){
            mc.beginBitmapFill(bmp,mx,true);
            mc.lineTo(wd,0);mc.lineTo(wd,hg);
            mc.lineTo(0,hg);mc.lineTo(0,0);
            mc.endFill();
        }
        draw_bmp(this,'bar_texture',bar_w,bar_h);
        function draw_desc_bmp(mc,dbl){
            draw_bmp(mc,dbl,button_lat,button_lat);
        }
        //
        function add_button(button_name,description){
            var nm='bar_'+button_name;
            var mc=createEmptyMovieClip(button_name,getNextHighestDepth());
            draw_desc_bmp(mc,nm);
            var mv=_parent.attachMovie('barDescription',nm,_parent.getNextHighestDepth());
            mv['desc']=description;mv._visible=false;mv['rel']=mc;
            //
            mc['description']=mv;
            mc.onRollOver=function(){
                this.description._visible=true;
            }
            mc.onRollOut=function(){
                this.description._visible=false;
            }
        }
    )");
    action_sprite(presprite,R"(
        add_button('home','Back to home');
        home.onPress=_root.homeBack;
    )");
    bar_coord+=button_szAndInter;
    actionf_sprite(presprite,buf,R"(
        add_button('sound','Music on/off');
        sound._%c=%u;
        //
        var sound_on_bmp=bmp;
        var sound_off_bmp=flash.display.BitmapData.loadBitmap('bar_sound_off');
        sound['on']=true;
        sound.onPress=function(){
            if(this.on==true){
                this.on=false;_root.game.music.stop();
                fill_mc(sound,sound_off_bmp,button_lat,button_lat);
            }else{
                this.on=true;_root.game.music.start();
                fill_mc(sound,sound_on_bmp,button_lat,button_lat);
            }
        }
    )",(bar_x<bar_y?'x':'y'),bar_coord);
    bar_coord+=button_szAndInter;
    actionf_sprite(presprite,buf,R"(
        add_button('keyboard','Set keys');
        keyboard._%c=%u;
        //
        keyboard.onPress=function(){
            _root.launch_keys_panel();
        }
    )",(bar_x<bar_y?'x':'y'),bar_coord);
    int bar_startPos=bar_coord+button_szAndInter;
    action_sprite(presprite,"delete bmp;");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bar");
    //barDescription
    presprite=swf_sprite_new();
    //
    int bar_text_h=2*button_sz[1];int bar_text_w=150;
    EditText bar_ed;bar_ed.fontid=defFont;bar_ed.font_height=button_sz[1];
    int txt=swf_text(bar_text_w,bar_text_h,"message",(HasFont|ReadOnly|NoSelect),&bar_ed);
    swf_sprite_placeobject(presprite,txt,0);
    //
    actionf_sprite(presprite,buf,"var text_h=%u;var off=%u",bar_text_h,bar_text_w);
    action_sprite(presprite,R"(
        var rel_bounds=rel.getBounds(_parent);_x=rel_bounds.xMin;_y=rel_bounds.yMin;
        _x-=off;
        beginFill(0xffFFff);
        lineTo(off,0);lineTo(off,text_h);
        lineTo(0,text_h);lineTo(0,0);
        endFill();
        message=desc;
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"barDescription");

    //

    //space env
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var wd=%u;var hg=%u",width,height);
    action_sprite(presprite,R"(
        var env_matrix=new flash.geom.Matrix();
        beginBitmapFill(flash.display.BitmapData.loadBitmap('space_env_bmp'),env_matrix,true);
        lineTo(wd,0);lineTo(wd,hg);
        lineTo(0,hg);lineTo(0,0);
        endFill();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"main_space_env");

    //

    //set keys
    action(R"(
        function launch_keys_panel(){
            if(keys_panel==undefined)attachMovie('keys_panel','keys_panel',getNextHighestDepth());
        }
    )");
    //
    action("var text_names=new Array();var key_names=new Array();var key_default_code=new Array()");
    int key_nr=0;while(key_nr<keyboards_total){
        actionf(buf,"text_names[%u]='%s';key_names[%u]='%s'",key_nr,keyboards[key_nr]->disp_name,key_nr,keyboards[key_nr]->name);
        actionf(buf,"key_default_code[%u]=%s",key_nr,get_def_code(keyboards[key_nr]->defaultKey));
        key_nr++;
    }
    #define key_text_w 200
    #define key_w 2*key_text_w
    #define key_unit_h 20
    #define key_unit_w key_w
    int key_h=key_unit_h*key_nr;
    int key_x=(width-key_w)/2;
    int key_y=(height-key_h)/2;
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"_x=%u;_y=%u",key_x,key_y);
    actionf_sprite(presprite,buf,"var key_unit_h=%u;var key_nr=%u",key_unit_h,key_nr);
    action_sprite(presprite,R"(
        var com=_root['shared_level'];
        var i;var key_nr=_root.text_names.length;
        i=0;while(i<key_nr){
            if(com[_root.key_names[i]]==undefined)com[_root.key_names[i]]=_root.key_default_code[i];
            i++;
        }
        //
        var new_key_mc;
        var yPos=0;
        i=0;while(i<key_nr){
            var d=getNextHighestDepth();
            var mc=attachMovie('keys_panel_entry','key'+d,d);mc._y=yPos;
            mc['text_name']=_root.text_names[i];
            mc['key_name']=_root.key_names[i];
            mc.onPress=function(){
                this.right_text='';
                new_key_mc=this;
            }
            yPos+=key_unit_h;i++;
        }
        var mc=attachMovie('keys_panel_ok','ok',getNextHighestDepth());mc._y=yPos;
        Key.addListener(this);
        mc.onPress=function(){
            Key.removeListener(this);
            removeMovieClip();
        }
        onKeyDown=function(){
            if(new_key_mc!=undefined){
                var code=Key.getCode();
                var key_id=new_key_mc.key_name;
                com[key_id]=code;
                new_key_mc.right_text=new_key_mc.keycode_to_string(code);
                new_key_mc=undefined;
                //
                com.so_keys.data[key_id]=code;
                com.so_keys.flush();
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"keys_panel");
    //
    actionf(buf,"var set_key_w=%u;var set_key_h=%u",key_unit_w,key_unit_h);
    action(R"(
        function set_key_fill(mc){
            mc.lineStyle(1,0xff0000);
            mc.beginFill(0x8080ff);
            mc.lineTo(set_key_w,0);mc.lineTo(set_key_w,set_key_h);
            mc.lineTo(0,set_key_h);mc.lineTo(0,0);
            mc.endFill();
        }
    )");
    int keys_text_h=key_unit_h-5;
    int text_entry;
    EditText keys_text;keys_text.fontid=defFont;keys_text.font_height=keys_text_h;
    //ok
    #define ok_x key_unit_w/2
    EditText ok_text;
    ok_text.fontid=defFont;ok_text.font_height=keys_text_h;
    ok_text.rgba=0xff0000FF;
    presprite=swf_sprite_new();
    text_entry=swf_text(key_unit_w-ok_x,key_unit_h,"ok_text",(HasFont|HasTextColor|ReadOnly|NoSelect),&ok_text);
    swf_sprite_placeobject_coords(presprite,text_entry,0,ok_x,0);
    action_sprite(presprite,"_root.set_key_fill(this);ok_text='OK'");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"keys_panel_ok");
    //
    presprite=swf_sprite_new();
        //
    text_entry=swf_text(key_text_w,key_unit_h,"left_text",(HasFont|ReadOnly|NoSelect),&keys_text);
    swf_sprite_placeobject(presprite,text_entry,0);
        //
    text_entry=swf_text(key_text_w,key_unit_h,"right_text",(HasFont|ReadOnly|NoSelect),&keys_text);
    swf_sprite_placeobject_coords(presprite,text_entry,1,key_text_w,0);
        //
    action_sprite(presprite,R"(
        var com=_root['shared_level'];
        //
        _root.set_key_fill(this);
        //
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
        left_text=text_name;
        right_text=keycode_to_string(com[key_name]);
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"keys_panel_entry");

    //scenario_info
    #define scenario_info_w 400
    #define scenario_info_x (width-scenario_info_w)/2
    #define scenario_info_center scenario_info_w/2
    //
    #define scenario_info_unit_w scenario_info_w
    #define scenario_info_unit_h 40
    //
    //#define scenario_info_unit_w scenario_info_w
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"_x=%u",scenario_info_x);
    actionf_sprite(presprite,buf,"var height=%u;var wd=%u;var oneLine_h=%u",height,scenario_info_unit_w,scenario_info_unit_h);
    actionf_sprite(presprite,buf,"var centerX=%u",scenario_info_center);
    #define infoText_h scenario_info_unit_h/2
    actionf_sprite(presprite,buf,"var textHeight=%u",infoText_h);
    actionf_sprite(presprite,buf,R"(
        var total_h=0;var mc;
        //mc
        function add_mc_base(textH,txt,wd,hg){
            var d=getNextHighestDepth();
            var mc=attachMovie('info_text','info_text'+d,d);
            mc['textHeight']=textH;
            mc['txt']=txt;mc['wd']=wd;mc['hg']=hg;
            mc._y=total_h;
            return mc;
        }
        //mc
        function add_mc_top(txt,hg){
            add_mc_base(textHeight,txt,wd,hg);
            total_h+=hg;
        }
        //mc
        function add_mc_left(txt){
            add_mc_base(disp_textH,txt,wd,oneLine_h);
        }
        //mc
        function add_mc_right(txt){
            var mc=add_mc_base(disp_textH,txt,wd-centerX,oneLine_h);
            mc._x=centerX;
            total_h+=oneLine_h;
        }
        //
        add_mc_top(_root.strip_underscores(_root.singleTraining[pos]),oneLine_h);
        //
		//A positive integer that specifies the height of the new text field. oneLine_h=40
        var desc_hg=oneLine_h*3+(oneLine_h*3/4);
        add_mc_top(_root.singleTraining_desc[pos],desc_hg);
        //
        var dispKey=_root.singleTraining_dispKey[pos];var descKey=_root.singleTraining_descKey[pos];
        var disp_textH=textHeight-5;
        var i=0;while(i<dispKey.length){
            add_mc_left(_root.singleTraining_dispKey[pos][i]);
            //
            add_mc_right(_root.singleTraining_descKey[pos][i]);
            //
            i++;
        }
        if(_root.singleTraining_mouse[pos]!=undefined){
            add_mc_left("%s");
            add_mc_right(_root.singleTraining_mouse[pos]);
        }
        //
        mc=attachMovie('info_button','info_button',getNextHighestDepth());
        mc._y=total_h;
        mc['txt']='Play';
        mc.onPress=function(){
            _root.start_scenario(pos);
        }
        total_h+=oneLine_h;
        //
        _y=(height-total_h)/2;
    )", mouse_disp_name);
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"scenario_info");
    //info_button
    EditText infoText;infoText.fontid=defFont;infoText.font_height=infoText_h;
    infoText.rgba=0xff0000FF;
    int infoTxt_entry;
    presprite=swf_sprite_new();
    infoTxt_entry=swf_text(scenario_info_unit_w-scenario_info_center,scenario_info_unit_h,"message",(HasFont|ReadOnly|NoSelect|HasTextColor),&infoText);
    swf_sprite_placeobject_coords(presprite,infoTxt_entry,0,scenario_info_center,0);
    actionf_sprite(presprite,buf,"_root.draw_list_entry(this,%u,%u);message=txt",scenario_info_unit_w,scenario_info_unit_h);
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"info_button");
    //info_text
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        _root.draw_list_entry(this,wd,hg);
        createTextField('txt_inf',getNextHighestDepth(),0,0,wd,hg);
        var fmt=new TextFormat();
        fmt.size=textHeight;
        txt_inf.multiline=true;
        txt_inf.wordWrap=true;
        txt_inf.text=txt;
        txt_inf.setTextFormat(fmt);
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"info_text");

    //rating
    presprite=swf_sprite_new();
    int star_sz[2];
    int star_empty_dbl=swf_dbl_ex("C:/Desktop/universe/img/root/empty_star.dbl",star_sz);
    int star_empty=swf_shape_bitmap(star_empty_dbl,star_sz[0],star_sz[1]);
    swf_sprite_placeobject(presprite,star_empty,0);
    action_sprite(presprite,"stop()");
    swf_sprite_showframe(presprite);
    int star_full_dbl=swf_dbl("C:/Desktop/universe/img/root/full_star.dbl");
    int star_full=swf_shape_bitmap(star_full_dbl,star_sz[0],star_sz[1]);
    swf_sprite_placeobject(presprite,star_full,1);swf_sprite_showframe(presprite);
    int star_wait_dbl=swf_dbl("C:/Desktop/universe/img/root/wait.dbl");
    int star_wait=swf_shape_bitmap(star_wait_dbl,star_sz[0],star_sz[1]);
    swf_sprite_placeobject(presprite,star_wait,2);
    swf_sprite_removeobject(presprite,0);swf_sprite_removeobject(presprite,1);
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"rating_star");
    //scenario_done
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"_x=%u",scenario_info_x);
    actionf_sprite(presprite,buf,"var height=%u;var wd=%u;var oneLine_h=%u",height,scenario_info_unit_w,scenario_info_unit_h);
    actionf_sprite(presprite,buf,"var textHeight=%u",infoText_h);
    actionf_sprite(presprite,buf,"var rate_w=%u;var rate_h=%u",star_sz[0],star_sz[1]);
	action_sprite(presprite, R"(
        var yPos=0;
        function add_mc(txt){
            var d=getNextHighestDepth();
            var mc=attachMovie('info_text','info_text'+d,d);
            mc['wd']=wd;mc['hg']=oneLine_h;
            mc['txt']=txt;mc['textHeight']=textHeight;
            mc._y=yPos;
            yPos+=oneLine_h;
            return mc;
        }
        ////
        add_mc('Level Done');
        add_mc('Time: '+time_sec+' seconds');
        //
	)");
	actionf_sprite(presprite, buf, R"(
        var rating_stars=add_mc('Rating:');
        rating_stars.onEnterFrame=function(){
			//dupa, pentru stele peste text pentru onPress
            this.onEnterFrame=null;
            
			function light_stars(n){
                for(i=1;i<=n;i++){rating_stars["star"+i].gotoAndStop(2);}
                for(i=n+1;i<=5;i++){rating_stars["star"+i].gotoAndStop(1);}
            }
			var stars=this;
            var load_rate=new LoadVars();
            load_rate.onData=function(src){
                if(src!=undefined){
					var rate=_root.parse_score(src);
					var rating_y=(oneLine_h-rate_h)/2;var rating_x=wd-10;
                    for(var i=0;i<5;i++){
						var n=5-i;
						var rt=stars.attachMovie("rating_star","star"+n,stars.getNextHighestDepth());
                        rt._y=rating_y;
                        rating_x-=rate_w;rt._x=rating_x;
                        rt['rate']=n;
						rt.onPress=function(){
							var n=this['rate'];
							var c_frame=this._currentframe;var this_star=this;this.gotoAndStop(3);
							var rate_get=new LoadVars();
							rate_get.onData=function(src){
								this_star.gotoAndStop(c_frame);
                                if(src!=undefined){
									_root.put_server(name_strip,undefined,n);
									light_stars(n);

									var rt=_root.parse_rate(src);
									if(rate==0)rt.users++;rt.sum+=n-rate;
									rate=n;
									var sc=_root.form_rate(rt);
									_root.put_server(name_strip,'%s',sc);
								}
							}
							var url_name=_root.form_stat_retrive(_root.singleTraining_stat[episode_number],_root.rats_id);
							rate_get.load(url_name);
						}
                    }
					light_stars(rate);
				}
            };
			var get_the_rate=new LoadVars();
			var urlname=_root.form_stat_retrive(_root.singleTraining_stat[episode_number],_global.kongregate.services.getUserId());
			get_the_rate.sendAndLoad(urlname,load_rate);
        }
	)", rat);
	action_sprite(presprite, R"(
        ////
        var mc=attachMovie('info_button','info_button',getNextHighestDepth());
        mc._y=yPos;
        mc['txt']='OK';
        mc.onPress=_root.homeAgain;
        yPos+=oneLine_h;
        //
        _y=(height-yPos)/2;
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"scenario_done");

    //counterBar
    int bar_sz=(bar_x<bar_y?bar_w:bar_h)-bar_startPos;
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var bar_sz=%u",bar_sz);
    char* cp_0=bar_x<bar_y?"current_pos":"0";
    char* _0_cp=bar_x<bar_y?"0":"current_pos";
    actionf_sprite(presprite,buf,R"(
        function draw_progressBar(){
            var current_pos=bar_sz*barNr/barMax;
            clear();
            beginFill(0x00ffFF);
            moveTo(%s,%s);
            lineTo(_root.bar._width,%s);
            lineTo(_root.bar._width,_root.bar._height);
            lineTo(%s,_root.bar._height);
            endFill();
        }
        draw_progressBar();
    )",cp_0,_0_cp,_0_cp,cp_0);
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"counterBar");
    //
    actionf(buf,R"(
        function counterBar_init(max_pos){
            var mc=bar.attachMovie('counterBar','counterBar',bar.getNextHighestDepth());
            mc._%s=%u;mc['barNr']=0;mc['barMax']=max_pos;
        }
    )",(bar_x<bar_y?"x":"y"),bar_startPos);
	action(R"(
        function counterBar_step(){
			counterBar_jump(1);
        }
		function counterBar_jump(a){
            bar.counterBar.barNr+=a;
            if((bar.counterBar.barNr<bar.counterBar.barMax)==false)end_scenario();
			else bar.counterBar.draw_progressBar();
        }
    )");

    //list
    dbl=swf_dbl("C:/Desktop/universe/img/root/list.dbl");
    swf_exports_add(dbl,"list_texture");
    actionf(buf,"var list_lineSz=%u",list_lineSz);
    action(R"(
        var list_matrix=new flash.geom.Matrix();
        var list_bmp=flash.display.BitmapData.loadBitmap('list_texture');
        function draw_list_entry(mc,wd,hg){
            draw_list_entry_ex(mc,wd,hg,0x0000ff);
        }
        function draw_list_entry_ex(mc,wd,hg,col){
            mc.lineStyle(list_lineSz,col);
            mc.beginBitmapFill(list_bmp,list_matrix,true);
            var marginVal=list_lineSz/2;
            mc.moveTo(marginVal,marginVal);
            mc.lineTo(wd-marginVal,marginVal);mc.lineTo(wd-marginVal,hg-marginVal);
            mc.lineTo(marginVal,hg-marginVal);mc.lineTo(marginVal,marginVal);
            mc.endFill();
        }
    )");
#define l_view "list_view"
    actionf(buf,R"(
        function list_loaded(){
            game.attachMovie('%s','%s',game.getNextHighestDepth());
        }
    )", l_view, l_view);
    //
    
    #define list_inset 100
    int list_w=width-(2*list_inset);
    int list_h=height-(2*list_inset);
    int list_unit_h=list_h/lista_ferestre_pagina;
    int list_unit_w=list_w;
    int text_height=list_unit_h-static_text_off_subtract;
    int list_view_presprite=swf_sprite_new();
    EditText ed;ed.fontid=defFont;ed.font_height=text_height;
    actionf_sprite(list_view_presprite,buf,"var list_inset=%u;var list_unit_w=%u;var list_unit_h=%u;var list_units=%u"
                ,list_inset         ,list_unit_w     ,list_unit_h        ,episoade_pagina
    );
    //list_entry
    #define list_unit_play_inset 5
    #define list_unit_play_h list_unit_h-(2*list_unit_play_inset)
    #define list_unit_play_w list_unit_play_h
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var list_unit_w=%u;var list_unit_h=%u",list_unit_w,list_unit_h);
    actionf_sprite(presprite,buf,"var play_inset=%u;var button_w=%u;var button_h=%u",list_unit_play_inset,list_unit_play_w,list_unit_play_h);
	action_sprite(presprite, R"(
        if(isSelected==true)_root.draw_list_entry_ex(this,list_unit_w,list_unit_h,0xff0000);
        else _root.draw_list_entry(this,list_unit_w,list_unit_h);
        //
        attachMovie('list_entry_play','play',getNextHighestDepth());
        play._x=list_unit_w-(2*play_inset)-button_w;play._y=(list_unit_h-button_h)/2;play['pos']=pos;
		//
		var xPos=play._x-10-button_w;
		//
		if(_root['singleTraining_scores'].length>0){
			createTextField('textF',getNextHighestDepth(),0,0,xPos,list_unit_h);
			textF.selectable=false;
			var fmt=new TextFormat();fmt.size=list_unit_h-10;fmt.align='right';
			textF.text=_root.singleTraining_scores[pos];
			var dotpos=textF.text.indexOf('.');
			if(dotpos!=-1)textF.text=textF.text.substr(0,dotpos)+textF.text.substr(dotpos,2);
			textF.setTextFormat(fmt);
		}
        //
        if(levelDone==true){
            var yPos=play._y;
            lineStyle(1,0xff0000);
            moveTo(xPos,yPos+button_h-(button_h/4));
            lineTo(xPos+(button_w/4),yPos+button_h);
            lineTo(xPos+button_w,yPos);
        }
    )");
    #define text_x list_unit_w/4
    #define rest_x list_unit_w-text_x
    int list_txt=swf_text(rest_x,list_unit_h,"message",(HasFont|ReadOnly|NoSelect),&ed);
    swf_sprite_placeobject_coords(presprite,list_txt,0,text_x,0);
    action_sprite(presprite,"message=_root.strip_underscores(_name)");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"list_entry");
    //list_entry_play
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var play_w=%u;var play_h=%u",list_unit_play_w,list_unit_play_h);
    action_sprite(presprite,R"(
        lineStyle(1);
        beginFill(0x0000ff);
        lineTo(play_w,play_h/2);lineTo(0,play_h);lineTo(0,0);
        endFill();
        //
        onPress=function(){
            _root.start_training(pos);
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"list_entry_play");
    //
    //list_pages
    //
    #define go_width 38
    presprite=swf_sprite_new();
    EditText ed_go;ed_go.fontid=defFont;ed_go.font_height=text_height;
    ed_go.initialtext="Go";ed_go.rgba=0xff0000FF;
    int list_go_txt=swf_text(go_width,list_unit_h,"",(HasFont|ReadOnly|NoSelect|HasText|HasTextColor),&ed_go);
    swf_sprite_placeobject(presprite,list_go_txt,0);
	action_sprite(presprite, R"(
		var page_max=_root.page_max;
        onPress=function(){
            var nr=Number(_parent.page_setNr);
            if(isNaN(nr)!=false)nr=page_max;
            else{
                nr=int(nr)-1;
                if(nr<0)nr=0;else if(page_max<nr)nr=page_max;
            }
            _root['list_mc'].put_page(nr);
        }
    )");
    //
    #define border_off 4
    actionf_sprite(presprite,buf,R"(
        lineStyle(1,0xff0000);
        var w_dist_to_underline=%u;
        var h_dist_to_underline=%u;
        var h_off=%u;
        moveTo(0,h_off);
        lineTo(w_dist_to_underline,h_off);
        lineTo(w_dist_to_underline,h_dist_to_underline);
        lineTo(0,h_dist_to_underline);
        lineTo(0,h_off);
    )",go_width,text_height+5,border_off);
    //
    swf_sprite_showframe(presprite);
    int go_button=swf_sprite_done(presprite);
    //continue_play
    presprite=swf_sprite_new();
    EditText ed_continue_play;ed_continue_play.fontid=defFont;ed_continue_play.font_height=text_height;
    ed_continue_play.initialtext="Play";ed_continue_play.rgba=0xff0000FF;
    #define continue_play_width 50
    int list_continue_play_txt=swf_text(continue_play_width,list_unit_h,"",(HasFont|ReadOnly|NoSelect|HasText|HasTextColor),&ed_continue_play);
    swf_sprite_placeobject(presprite,list_continue_play_txt,0);
    action_sprite(presprite,R"(
        onPress=function(){
            _root.start_scenario(pos);
        }
    )");
    //
    actionf_sprite(presprite,buf,R"(
        lineStyle(1,0xff0000);
        var w_dist_to_underline=%u;
        var h_dist_to_underline=%u;
        var h_off=%u;
        moveTo(0,h_off);
        lineTo(w_dist_to_underline,h_off);
        lineTo(w_dist_to_underline,h_dist_to_underline);
        lineTo(0,h_dist_to_underline);
        lineTo(0,h_off);
    )",continue_play_width,text_height+5,border_off);
    //
    swf_sprite_showframe(presprite);
    int continue_play=swf_sprite_done(presprite);
    swf_exports_add(continue_play,"list_continue_play");
    //
    presprite=swf_sprite_new();
    //
    EditText ed_inf;ed_inf.fontid=defFont;ed_inf.font_height=text_height;
    int list_inf_txt=swf_text(list_unit_w/4,list_unit_h,"page_info",(HasFont|ReadOnly|NoSelect),&ed_inf);
    swf_sprite_placeobject(presprite,list_inf_txt,0);
    //
    #define nr_size 40
    int setNr_x=list_unit_w/8;
    int go_x=setNr_x+nr_size;
    #define setNr_inset list_lineSz
    EditText ed_setnr;ed_setnr.fontid=defFont;ed_setnr.font_height=text_height-(2*setNr_inset);
    int list_nr_txt=swf_text(nr_size,list_unit_h-(2*setNr_inset),"page_setNr",(HasFont|Border),&ed_setnr);
    swf_sprite_placeobject_coords(presprite,list_nr_txt,1,setNr_x,setNr_inset);
    //
    swf_sprite_placeobject_coords(presprite,go_button,2,go_x+3,0);
    //
    actionf_sprite(presprite,buf,"_root.draw_list_entry(this,%u,%u)",list_unit_w,list_unit_h);
    actionf_sprite(presprite,buf,R"(
        if(continue_pos!=undefined){
            attachMovie('list_continue_play','continue_play',getNextHighestDepth());
            continue_play._x=%u;
            continue_play['pos']=continue_pos;
        }
    )",(go_x+go_width+100));
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"list_pages");
    //
	action_sprite(list_view_presprite, "var list_units_all=_root.list_units_all;var page_max=_root.page_max");
    action_sprite(list_view_presprite,R"(
        _root['list_mc']=this;
        function put_page(page_nr){
            var d=container.getDepth();
            container.removeMovieClip();
            createEmptyMovieClip('container',d);
            //
            var list_start=page_nr*list_units;
            //                      ultima din pagina poate fi list_start+list_units sau ultimul level
            var list_end=Math.min(list_start+list_units,list_units_all);
            //
            var yPos=0;var mc;
            //la ultimele sa arate si dinainte sa fie pagina plina
            if((list_end-list_start)<list_units)list_start=list_units_all-list_units;
            //
            for(var j=list_start;j<list_end;j++){
                var i=_root.sorter[j];
                mc=container.attachMovie('list_entry',_root.singleTraining[i],container.getNextHighestDepth());
                mc._y=yPos;yPos+=list_unit_h;mc['pos']=i;
                if(j==selLevel)mc['isSelected']=true;
                else if(so.data['lev'+i]==true)mc['levelDone']=true;
            }
            mc=container.attachMovie('list_pages','list_pages',container.getNextHighestDepth());
            mc._y=yPos;
            mc['page_info']=(page_nr+1)+'/'+(page_max+1);
            //remarcare: Click to play e pe bara unde e si "Go", bara e pe toata pagina, care se schimba
            if(selLevel<list_units_all)mc['continue_pos']=_root.sorter[selLevel];
        }
    )");
    //sort
    #define sort_h list_unit_h
    #define sort_w list_unit_w/sorts_length
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var wd=%u;var hg=%u;var w=%u;var line=%u",sort_w,sort_h,sort_w-4,list_lineSz);
	action_sprite(presprite, R"(
        var list_matrix=new flash.geom.Matrix();
        var list_bmp=flash.display.BitmapData.loadBitmap('list_texture');
        _y-=hg;
        _x=coef*wd;
        function paintings(){
            clear();
            beginBitmapFill(list_bmp,list_matrix,true);
            lineTo(w,0);lineTo(w,hg);lineTo(0,hg);
            endFill();
            if(shar.data.value==_name){
                beginBitmapFill(list_bmp,list_matrix,true);
                moveTo(0,hg);
                lineTo(w,hg);lineTo(w,hg+line);lineTo(0,hg+line);
                endFill();
                //
				_root.call(_name,_parent);
            }
        }
        var shar=SharedObject.getLocal('listSort');
        paintings();
        onPress=function(){
            var prev=_parent[shar.data.value];
            shar.data.value=_name;
            shar.flush();
			_root.homeAgain();
        }
    )");
    #define sort_text_offX 10
    list_txt=swf_text(sort_w-sort_text_offX,sort_h,"message",(HasFont|ReadOnly|NoSelect),&ed);
    swf_sprite_placeobject_coords(presprite,list_txt,0,sort_text_offX,0);
    action_sprite(presprite,"message=_name");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"list_sort");
    //put container then sorters above, to draw sorter over list
    action_sprite(list_view_presprite,"createEmptyMovieClip('container',getNextHighestDepth())");
    for(int i=0;i<sorts_length;i++){
        actionf_sprite(list_view_presprite,buf,"var mov=attachMovie('list_sort','%s',getNextHighestDepth());mov['coef']=%u",sorts[i],i);
    }
    //
    action_sprite(list_view_presprite,R"(
        _x=list_inset;_y=list_inset;
        //
        var so=SharedObject.getLocal('levelIsDone');
        var selLevel;
        function first_page(){
            selLevel=0;
            for(;selLevel<list_units_all;selLevel++)if(so.data['lev'+_root.sorter[selLevel]]!=true)break;
            //              aicea e player level sau ultimul index
            put_page(int(Math.min(selLevel,list_units_all-1)/list_units));
        }
    )");
    swf_sprite_showframe(list_view_presprite);
    sprite=swf_sprite_done(list_view_presprite);swf_exports_add(sprite, l_view);

    //

    actionf(buf,R"(
        var sharlistSort=SharedObject.getLocal('listSort');
        if(sharlistSort.data.value==undefined)sharlistSort.data['value']='%s';
        //if(sharlistSort.data.timeout==undefined)sharlistSort.data['timeout']=9;
		//timeout apare ciudat in fisierul sol, dar e corect
    )",sorts[0]);
	action(R"(
        //var gameDomain='ajob.atwebpages.com';
        //127.0.0.1
        function load_swf(swf_name,instance){
            instance.loadMovie(swf_name);
        }
        function home_set(){
            game.attachMovie('main_space_env','space_env',game.getNextHighestDepth());
            //
            game.createEmptyMovieClip('list_movie',game.getNextHighestDepth());

			//list_movie continut devine cel din list.swf
            //allow_domain('http://universe-game.com/list.swf',game.list_movie);
            load_swf('list.swf',game.list_movie);
        }
		function game_clear(){
			var d=game.getDepth();
            game.removeMovieClip();
            createEmptyMovieClip('game',d);
		}
		//game reset vine de la home press si end scenario
        function game_reset(){
			//doar cand e cazul
			if(automatics_func!=undefined){
				automatics_func();
				automatics_func=undefined;
			}
			//
            game.music.stop();
            game_clear();
			//doar cand e cazul
            bar.counterBar.removeMovieClip();
			//
        }
        function homeBack(){
            game_reset();
            home_set();
        }
		//de la end list ok, de la new sort
		function homeAgain(){
			game_clear();
			home_set();
		}
        function start_training(pos){
            game_clear();
            shared_level.space_env(game,'env',game.getNextHighestDepth());
            game.attachMovie('scenario_info','scenario_info',game.getNextHighestDepth());
            game.scenario_info['pos']=pos;
        }
        function strip_underscores(txt){
            var splitAr=txt.split('_');
            return splitAr.join(' ');
        }
	)");
	actionf(buf, R"(
        var start_time;
        function start_scenario(pos){
            game_clear();
            
			var statista='%s';
			var loader=new LoadVars();
			loader.onData=function(data){
				if(data!=undefined){
					var plays=parse_score(data);
					put_server(strip_underscores(singleTraining[pos]),statista,plays+1);
				}
			}
			loader.load(form_stat_retrive(singleTraining_stat[pos],stat_id(statista)));

			var ep_name=singleTraining[pos];
			game['episode_number']=pos;
			//inca un level in jos ca altfel la load movie se elimina continutul actual
            game.createEmptyMovieClip('scenario',game.getNextHighestDepth());

            //tm e ca pe kongregate nu vin fisierele noi cand se incearca updatarea
            //e timpul fisierului si il ia pe cel nou
            //dar se trimite in milisecunde dar parca acolo e in secunde
            //var tm=new Date();
            //game.scenario.loadMovie(ep_name+'.swf?'+tm.valueOf());
            //game.scenario.loadMovie('http://universe-game.com/'+ep_name+'.swf');
            //game.scenario.loadMovie(ep_name+'.swf');
            load_swf(ep_name+'.swf',game.scenario);
            //+'?9999999999999'
            //game.scenario.loadMovie(ep_name+'.swf'+'?9999999999999');
            //asta trebuie cand e cu www si de la kongregate
            //89.35.62.10
            //allow_domain('http://universe-game.com/test1.swf',game.scenario);
            //game.scenario.loadMovie('http://universe-game.com/test1.swf');

            game.music=new Sound();
            game.music.onLoad=function(success){
                if(bar.sound.on==true)this.start();
            }
            game.music.onSoundComplete=function(){
                this.start();
            }
            //tm e ca pe kongregate nu vin fisierele noi cand se incearca updatarea
            //game.music.loadSound(ep_name+'.mp3?'+tm.valueOf(),false);
            //game.music.loadSound('http://'+gameDomain+'/'+ep_name+'.music',false);
			game.music.loadSound(ep_name+'.mp3',false);
            //
            //game.music.loadSound('http://universe-game.com/'+ep_name+'.mp3',false);
            
            start_time=new Date();
        }
	)", pop);
	action(R"(
        function end_scenario(){
            var pos=game.episode_number;
			//game reset curata tot din "game"
            game_reset();

            shared_level.space_env(game,'env',game.getNextHighestDepth());
            game.attachMovie('scenario_done','scenario_done',game.getNextHighestDepth());
            //
            var end_time=new Date();
            game.scenario_done['time_sec']=int((end_time.valueOf()-start_time.valueOf())/1000);
			game.scenario_done['episode_number']=pos;
			var nstrip=strip_underscores(singleTraining[pos]);
			game.scenario_done['name_strip']=nstrip;

            //mai intai asta ca sa se observe daca este ultimul episod done ca sa se trimita game done
            var so=SharedObject.getLocal("levelIsDone");
            kongregate.stats.submit(nstrip,game.scenario_done.time_sec);
            //
            if(so.data['lev'+pos]!=true){
                var i=0;var eps_done=1;
                for(;i<singleTraining.length;i++){
                    if(so.data['lev'+i]==true)eps_done++;
                }
				//no Return value
                kongregate.stats.submit('zzzUniverse',eps_done);
            }
            //
            so.data['lev'+pos]=true;
            so.flush();
        }
        //asta trebuie, ca _global e undefined in loadMovie
        var kongregate=_global.kongregate;
        //if(kongregate!=undefined){
        kongregate.services.connect();
            //System.security.allowDomain('http://universe-game.com');
        //}
    )");
#define playsid 13150617
#define ratsid 13150685
	actionf(buf, R"(
		var rats_id=%u;
		function put_server(nm,t,value){
//informer(inform.info+' b '+value);
			var u_id=kongregate.services.getUserId();
//ca poate greseste cineva si intra cu playsid sau ratsid si joaca si strica tot
			if(u_id!=%u&&u_id!=rats_id){
				var vars=new LoadVars();
				vars.user_id=stat_id(t);
				vars.api_key='a2a76940-ec2c-4e4b-9bb9-81f5784d6d7c';
				vars['zz'+nm+' rating_plays']=value;
				//la send trebuie target _self _blank ...
				var lv=new LoadVars();
				//onLoad This handler is undefined by default.
				//lv.onLoad=function(b){if(b==false)informer('Error');}
				//doar cu primul parametru nu trimite
				vars.sendAndLoad('https://api.kongregate.com/api/submit_statistics.json',lv);
				//fara asta nu arata nimic: "Make sure you have display in leaderboards checked"
			}
		}
	)", ratsid,playsid);
	actionf(buf, R"(
		function stat_id(type){
			if(type=='%s')return %u;
			else if(type=='%s')return rats_id;
			return kongregate.services.getUserId();
		}
	)",pop, playsid, rat);
	actionf(buf,R"(
		function parse_score(data){
//BIG_INT in baza da date
			return new Number(parse_score_base(data));
		}
		function parse_score_base(data){
			var against='"score":';
			var pos=data.indexOf(against);
			if(pos==-1)return %s;
			var i=pos+against.length;var j=i;while(isNaN(data.charAt(j))==false)j++;
			return data.substr(i,j-i);
		}
	)", scor_bazic_string);
	action(R"(
		function call(n,mc){
			sorter=_root['sorter'+n];
			mc.first_page();
		}
		function form_stat_retrive(n,u){
			//www.kongregate nu trimite nimic
			return 'http://api.kongregate.com/api/high_scores/friends/'+n+'/'+u+'.json';
			//lifetime
		}
	)");
	actionf(buf, "var double_profit=%u", 10);//ca BIG_INT
	//922.337.203 si 10 la stele (*5=1 12E0 BE7F)
	action(R"(
//poate sunt deja 33.000.000 useri (http://www.kongregate.com/users/33000000/posts), nu e matematic sa las maxim 20.000.000, trec de la Number la BIG_INT
		function parse_rate(nr){
//chiar nu e frumoasa impartirea de numere double:1semn/11exponent/52mantisa
//"level":1,"score":123456789012,"avatar_url"
//Number integers, mantisa 52bit: -9,007,199,254,740,992 (-2^53) to     9.007.199.254.740.992 (2^53)
//The maximum value of a stat is BIG_INT (9.223 x 10^18)            9.223.372.036.854.775.807
			return parse_rate_base(parse_score_base(nr));
		}
		function parse_rate_base(str){
//tot codul acesta doar ca sa incapa si zero cand dau rating din joc pentru prima data
			var last=str.length-1;
			var u=str.charAt(0);var uu='';var s=str.charAt(last);
			for(var i=last-1;i>0;i--){
				var a=str.charAt(i);
				if(i<double_profit)s=a+s;
				else uu=a+uu;
			}
			//ActionToInteger nu este BIG_INT
			return new rate_concat(int(u+uu),new Number(s));
		}
		function form_rate(rt){
			var x=rt.sum.toString();var dif=double_profit-x.length;
			for(var i=0;i<dif;i++)x='0'+x;
			return rt.users.toString()+x;
		}
		function rate_concat(u,s){
			this.users=u;this.sum=s;
		}
	)");

	//com
	shared_write();

	//debug
	//informer
	presprite = swf_sprite_new();
	EditText edTx; edTx.fontid = defFont; edTx.font_height = 20; edTx.rgba = 0xffff00ff;
	int informer = swf_text(width, edTx.font_height * 2, information, HasFont | HasTextColor | ReadOnly | NoSelect, &edTx);
	swf_sprite_placeobject(presprite, informer, 0); swf_sprite_showframe(presprite);
	sprite = swf_sprite_done(presprite); swf_exports_add(sprite, information);
	actionf(buf, R"(
		function inf_zero(){
//nu se poate sterge daca se adauga in list.swf
			attachMovie('%s','%s',getNextHighestDepth());%s.n=0;
		}
		function inf_cut(){
//nu se poate sterge din list.swf
			%s.removeMovieClip();
		}
	)", information, information, information, information);//, information

	action("createEmptyMovieClip('game',getNextHighestDepth());home_set();attachMovie('bar','bar',getNextHighestDepth())");
	if (bar_x != 0)actionf(buf, "bar._x=%u", bar_x);
	if (bar_y != 0)actionf(buf, "bar._y=%u", bar_y);
	action("attachMovie('shared_level','shared_level',getNextHighestDepth())");

	/*action(R"(
        function infoOut(mc){
            mc.removeMovieClip();
        }
        function informer(s){
			if(inform!=undefined)clearTimeout(inform.timer);
			else{
				attachMovie('informer','inform',getNextHighestDepth());
				informer._x=_width/2;informer._y=_height/2;
			}
            inform.onEnterFrame=function(){
                this.onEnterFrame=undefined;
                this.info=s;
                this.timer=setTimeout(infoOut,3000,this);
            }
        }
    )");*/

    swf_exports_done();
	swf_showframe();
	swf_done();
	printf("\ndone");
	return 1;
}
#ifdef __cplusplus
}
#endif