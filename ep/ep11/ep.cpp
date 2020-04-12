
#include<windows.h>
#include<stdio.h>
#include<math.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

int _start(){
    char* ep_nm=Space_Trip_ep;
	sprintf(buf, "%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);
    action("_root['episode']=this");
    init_episod(ep_nm);

    //e ep ca e zone.onEnterFrame= si nu merge this in asta
    action("var ep=_root['episode']");

    //are buguri compilatorul cu widthul
    int wdt=width;

    //aicea ex: step 800/48=16.(6) si 20 este latura unei target
    int wd=wdt;wd/=3*fps;
    int bullet_step=wd;
    //
    #define y_step 3*ship_sz[1]/fps
    #define hit_slow y_step/2
    #define fast_env_step 0-y_step

    //ship
    //ship body main
    int ship_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/11/ship.dbl",ship_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_body_main");
    //ship body backFuelTube
    int backFuelTube_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/11/backFuelTube.dbl",backFuelTube_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_body_backFuelTube");
    //ship body diagFuelTube
    int diagFuelTube_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/11/diagFuelTube.dbl",diagFuelTube_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_body_diagFuelTube");
    //bullet
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var step=%u",bullet_step);
    actionf_sprite(presprite,buf,"var width=%u",width);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        var bullet_r=3;
        var color=0xff8000;
        beginFill(color);
        com.drawCircle_offset(this,bullet_r,0-bullet_r);
        endFill();
        //
        _x=ep.ship._x+ep.ship.ship_body._width+bullet_r;_y=ep.ship._y+(ep.ship.ship_body._height/2);
        onEnterFrame=function(){
            _x+=step;
            if(width<_x)removeMovieClip();
            else ep.bullet_action(this);
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bullet");
	//
	automatic(1);
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var ship_h=%u",ship_sz[1]);
    actionf_sprite(presprite,buf,"var backFuelTube_h=%u",backFuelTube_sz[1]);
    actionf_sprite(presprite,buf,"var diagFuelTube_w=%u;var diagFuelTube_h=%u",diagFuelTube_sz[0],diagFuelTube_sz[1]);
    actionf_sprite(presprite,buf,"var y_step=%u",y_step);
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    actionf_sprite(presprite,buf,"var shot_interval=%u",shot_interval);
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        //
        attachMovie('ship_body_main','ship_body',getNextHighestDepth());
        attachMovie('ship_body_backFuelTube','ship_body_backFuelTube',getNextHighestDepth());
        ship_body_backFuelTube._y=(ship_h-backFuelTube_h)/2;
        attachMovie('ship_body_diagFuelTube','ship_body_diagFuelTube_up',getNextHighestDepth());
        attachMovie('ship_body_diagFuelTube','ship_body_diagFuelTube_down',getNextHighestDepth());
        ship_body_diagFuelTube_down._rotation=-90;
        ship_body_diagFuelTube_down._y=ship_h;
        //
        function ship_fuel(name,FuelTube,x1,y1,x2,y2){
            var mc=attachMovie('fuel',name,getNextHighestDepth());
            var x3=x2-x1;var y3=y2-y1;
            //centru p1 si raza pana la p2 si + 90 grade si directia e de la mijloc cu acest unghi
            mc['angle']=Math.atan2(y3,x3)*180/Math.PI+90;
            mc._x=get_center(x1,x2);mc._y=get_center(y1,y2);
            mc['x1']=x1-mc._x;mc['y1']=y1-mc._y;
            mc['x2']=x2-mc._x;mc['y2']=y2-mc._y;
            mc._x+=FuelTube._x;mc._y+=FuelTube._y;
        }
        function get_center(a,b){
            var min=Math.min(a,b);
            var dist=Math.abs((a-b)/2);
            return min+dist;
        }
        ship_fuel('up_fuel',ship_body_diagFuelTube_up,diagFuelTube_w,0,0,diagFuelTube_h);
        ship_fuel('down_fuel',ship_body_diagFuelTube_down,0,0-diagFuelTube_h,diagFuelTube_w,0);
        ship_fuel('forward_fuel',ship_body_backFuelTube,0,0,0,backFuelTube_h);
        //
        var fuel_clips=new Object();
        fuel_clips['up_fuel']=new Array(down_fuel,forward_fuel);
        fuel_clips['down_fuel']=new Array(up_fuel,forward_fuel);
        fuel_clips['forward_fuel']=new Array(up_fuel,down_fuel);
        attachMovie('ship_body','ship_body',getNextHighestDepth());
		function shot_func(){
			if(shot_timer==undefined){
				var d=ep.bullets.getNextHighestDepth();
                ep.bullets.attachMovie('bullet','bullet'+d,d);
                //
                createEmptyMovieClip('shot_timer',getNextHighestDepth());
                shot_timer['shot_interval']=shot_interval;
                shot_timer.onEnterFrame=function(){
                    this.shot_interval--;if(this.shot_interval<0)this.removeMovieClip();
                }
            }
		}
        onEnterFrame=function(){
            if(Key.isDown(com.action_key)==true)shot_func();
			else if(ep.auto_shot==true)shot_func();
            if(Key.isDown(com.up_arrow)==true){
                fuel_listener('down_fuel');
                _y-=y_step;
                if(_y<(ep.up_margin._y+ep.up_margin._height))_y+=2*y_step;
            }
            else if(Key.isDown(com.down_arrow)==true){
                fuel_listener('up_fuel');
                _y+=y_step;
                if(ep.down_margin._y<(_y+ship_h))_y-=2*y_step;
            }
            else{
                fuel_listener('forward_fuel');
            }
            ep.env_x_move(0-y_step);
        }
        function fuel_listener(fuel){
            if(this[fuel].onEnterFrame==undefined){
                var mc=this.fuel_clips[fuel][0];
                mc.onEnterFrame=undefined;mc.clear();
                var mc=this.fuel_clips[fuel][1];
                mc.onEnterFrame=undefined;mc.clear();
                this[fuel].onEnterFrame=this[fuel].fuelOn;
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship");
#define fuel_max 20
    //fuel
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fuel_max=%u;var fps=%u",fuel_max,fps);
    action_sprite(presprite,R"(
        var min_dist=10;var max_dist=fuel_max*2;
        var step=max_dist/fps;var next_margin=max_dist;
        var dist_val=min_dist;
        //
        var colors=new Array(0xff0000,0xffFF00);
        var alphas=new Array(80,80);
        //folosesc setMember;new Array(200) e push 200,push 1,push 'Array',newobj  si la enumerare e nimic la flash player
        var ratios=new Array();ratios[0]=0;
        var fuel_matrix=new flash.geom.Matrix();
        //
        function fuelOn(){
            var a=dist_val<next_margin;
            dist_val+=step;
            if(a!=(dist_val<next_margin)){
                if(next_margin==max_dist)next_margin=min_dist;
                else next_margin=max_dist;
                step*=-1;
            }
            var x3=Math.cos(angle*Math.PI/180)*dist_val;
            var y3=Math.sin(angle*Math.PI/180)*dist_val;
            //dist_val e varful curvei unde nu ajunge desenul,las la o treime
            ratios[1]=255*(dist_val/3)/Stage.width;
            //
            clear();
            beginGradientFill('radial',colors,alphas,ratios,fuel_matrix);
            moveTo(x1,y1);lineTo(x2,y2);
            curveTo(x3,y3,x1,y1);
            endFill();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"fuel");

    //margin
    int margin_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/11/margin.dbl",margin_sz);
    swf_exports_add(dbl,"margin_texture");
    action("var margin_texture=flash.display.BitmapData.loadBitmap('margin_texture')");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u",width);
    actionf_sprite(presprite,buf,"var margin_w=%u;var margin_h=%u",margin_sz[0],margin_sz[1]);
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var margin_matrix=new flash.geom.Matrix();
        margin_matrix.d=d;
        function draw_margin(distance){
            margin_matrix.tx+=distance;
			if(margin_matrix.tx<(0-margin_w))margin_matrix.tx+=margin_w;
            //
            clear();
            beginBitmapFill(ep.margin_texture,margin_matrix,true);
            lineTo(width,0);lineTo(width,margin_h);
            lineTo(0,margin_h);lineTo(0,0);
            endFill();
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"margin");

    //obst
    int obst_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/11/obst.dbl",obst_sz);
    swf_exports_add(dbl,"obst_texture");
    action("var obst_texture=flash.display.BitmapData.loadBitmap('obst_texture')");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var obst_w=%u",obst_sz[0]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        if(no_hit!=true)onEnterFrame=ep.env_hitTest;
        if(w==undefined)w=obst_w;
        //
        var obst_matrix=new flash.geom.Matrix();
        beginBitmapFill(ep.obst_texture,obst_matrix,true);
        lineTo(w,0);lineTo(w,h);
        lineTo(0,h);lineTo(0,0);
        endFill();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"obst");

    //green
    int green_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/11/verde.dbl",green_sz);swf_sprite_placeobject(presprite,bmp,0);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        onEnterFrame=ep.env_hitTest;
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"green");

    //one2three
    presprite=swf_sprite_new();
    //dist e jumate si random e cealalta jumatate
    #define dist (height-(2*margin_sz[1]))/2
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var dist=%u",dist);
    actionf_sprite(presprite,buf,"var margin_h=%u",margin_sz[1]);
    actionf_sprite(presprite,buf,"var green_w=%u;var green_h=%u",green_sz[0],green_sz[1]);
    action_sprite(presprite,"var ep=_root['episode'];");
    //
    actionf_sprite(presprite,buf,"var bullet_step=%u",bullet_step);
    #define button_lat 10
    actionf_sprite(presprite,buf,"var button_lat=%u",button_lat);
    action_sprite(presprite,R"(
        function green_hitTest(){
            var bl=ep.bullets;
            var a=0;while(a<3){
                var mc=this['bullet'+a];
                var p=mc.getBounds(ep);
                var target_name=mc.target_name;
                mc.removeMovieClip();
                var d=bl.getNextHighestDepth();
                mc=bl.createEmptyMovieClip('bullet'+d,d);
                draw_bullet(mc,0x00ff00,p.xMin,p.yMin,a);
                mc['target_name']=target_name;
                mc.onEnterFrame=function(){
                    var target=ep.zone.targets[this.target_name];
                    if(target==undefined)this.removeMovieClip();
                    else{
                        var p=target.getBounds(this._parent);
                        var x=(p.xMin+(button_lat/2))-this._x;
                        var y=(p.yMin+(button_lat/2))-this._y;
                        var angle=Math.atan2(y,x)*180/Math.PI;
                        this._rotation=angle;
                        this._x+=Math.cos(angle*Math.PI/180)*bullet_step;this._y+=Math.sin(angle*Math.PI/180)*bullet_step;
                        ep.bullet_action(this);
                    }
                }
                a++;
            }
            this.hitTest_function=undefined;
        }
    )");
    //
    action_sprite(presprite,R"(
        //e isoscel si machine_to_obs este jumate din heightul figurii
        var machine_to_obst=dist/2;
        createEmptyMovieClip('targets',getNextHighestDepth());
        var x=width;
        //mc
        function add_button(a){
            d=targets.getNextHighestDepth();
            var mc=targets.attachMovie('button','button'+d,d);
            mc._x=x;
            if(a==0)mc._y=buttons_y;
            else if(a==1)mc._y=threeMachine_center_y;
            else mc._y=buttons_y+dist;
            return mc;
        }
        function draw_bullet(mv,color,x,y,a){
            mv._x=x;mv._y=y;
            mv.lineStyle(2,color);
            mv.lineTo(green_w/2,0);
            if(a==0)mv._rotation=-45;
            else if(a==2)mv._rotation=45;
        }
        //
        var i=0;while(i<10){
            var buttons_y=random(dist)+margin_h;
            var threeMachine_center_y=buttons_y+(dist/2);
            //
            var d=targets.getNextHighestDepth();
            var mc=targets.attachMovie('green','green'+d,d);
            mc._x=x;mc._y=threeMachine_center_y-(green_h/2);x+=green_w;
            mc.hitTest_function=green_hitTest;
            var a=0;while(a<3){
                d=getNextHighestDepth();
                var mv=createEmptyMovieClip('bullet'+d,d);
                draw_bullet(mv,0xffFFff,x,threeMachine_center_y,a);
                mc['bullet'+a]=mv;
                a++;
            }
            x+=green_w;
            x+=machine_to_obst;
            var d=getNextHighestDepth();
            var mv=attachMovie('obst','obst'+d,d);
            mv._x=x;mv._y=margin_h;mv['h']=height-(2*margin_h);
            //
            var mc0=add_button(0);var mc1=add_button(1);var mc2=add_button(2);
            var a=0;while(a<3){
                var clip=this['mc'+a];
                clip['obst']=mv;
                clip['name_A']=this['mc'+((a+1)%3)]._name;clip['name_B']=this['mc'+((a+2)%3)]._name;
                mc['bullet'+a]['target_name']=clip._name;
                a++;
            }
            //
            x+=300;
            i++;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"one2three");
    //button
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var button_lat=%u",button_lat);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var half=button_lat/2;
        beginFill(0x11aaff);moveTo(0,half);
        lineTo(0-button_lat,half);lineTo(0-button_lat,0-half);
        lineTo(0,0-half);lineTo(0,0);
        endFill();
        function hitTest_function(){
            var a=ep.zone.targets[name_A];
            if(a==undefined){
                var b=ep.zone.targets[name_B];
                if(b==undefined){
                    a.removeMovieClip();
                    b.removeMovieClip();
                    obst.removeMovieClip();
                }
            }
            removeMovieClip();
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"button");

    //simpleObst_zone
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var ship_h=%u",ship_sz[1]);
    action_sprite(presprite,R"(
        var height_half=height/2;
        var x=width;
        var i=random(2);var i_max=i+10;
        while(i<i_max){
            var d=getNextHighestDepth();
            var mc=attachMovie('obst','obst'+d,d);
            mc._x=x;
            mc._y=height_half*(i&1);
            mc['h']=height_half;
            x+=100;
            i++;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"simpleObstacles");

    //holes
    #define hole_lat 400
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var hole_lat=%u",hole_lat);
    action_sprite(presprite,R"(
        var x=width;
        var i=0;while(i<10){
            var d=getNextHighestDepth();
            var mc=attachMovie('hole','hole'+d,d);
            mc._x=x;mc._y=height/2-(hole_lat*(i&1));
            x+=hole_lat;
            i++;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"holes");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    actionf_sprite(presprite,buf,"var hole_lat=%u",hole_lat);
    actionf_sprite(presprite,buf,"var step=%u",y_step/2);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var rand_factor=random(2);
        var lines_nr=16;
        var inter_frames=0;
        //
        var half=hole_lat/2;
        var dist=20;
        onEnterFrame=function(){
            if(hitTest(ep.ship.ship_body)==true){
                var ship=ep.ship;var ship_body=ship.ship_body;
                var points=getBounds(ep);
                //
                var hole_x_cent=points.xMin+half;
                var ship_x_cent=ship._x+(ship_body._width/2);
                var x_dist=ship_x_cent-hole_x_cent;
                ep.env_x_move(step*x_dist/half);
                //
                var hole_y_cent=points.yMin+half;
                var ship_y_cent=ship._y+(ship_body._height/2);
                var y_val=hole_y_cent-ship_y_cent;
                ship._y+=step*y_val/half;
            }
            //
            inter_frames--;if(0<inter_frames)return null;inter_frames=fps/2;
            clear();
            lineStyle(1,0xffFFff,60);
            var i=0;while(i<lines_nr){
                if((i&1)==rand_factor){
                    var p1=random(half-dist);var p2=p1+dist;var p3=half-p2;
                    var rot=360*i/lines_nr;
                    moveTo(half,half);
                    var x=Math.cos(rot*Math.PI/180)*p1;var y=Math.sin(rot*Math.PI/180)*p1;
                    lineTo(half+x,half+y);
                    x=Math.cos(rot*Math.PI/180)*p2;y=Math.sin(rot*Math.PI/180)*p2;
                    moveTo(half+x,half+y);
                    x=Math.cos(rot*Math.PI/180)*p3;y=Math.sin(rot*Math.PI/180)*p3;
                    lineTo(half+x,half+y);
                }
                i++;
            }
            moveTo(0,half);lineTo(hole_lat,half);
            moveTo(half,0);lineTo(half,hole_lat);
            rand_factor^=1;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"hole");

    //redTargets
    presprite=swf_sprite_new();
    int red_sz[2];
    bmp=swf_image_ex("C:/Desktop/universe/img/11/rosu.dbl",red_sz);swf_sprite_placeobject(presprite,bmp,0);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        onEnterFrame=ep.env_hitTest;
        function hitTest_function(){
            removeMovieClip();
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"red_target");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var ship_h=%u",ship_sz[1]);
    actionf_sprite(presprite,buf,"var target_w=%u;var target_h=%u",red_sz[0],red_sz[1]);
    action_sprite(presprite,R"(
        var x=width;
        createEmptyMovieClip('targets',getNextHighestDepth());
        var i=0;while(i<20){
            var y_pos=0;while(y_pos<height){
                var d=targets.getNextHighestDepth();
                var mc=targets.attachMovie('red_target','red_targets'+d,d);
                mc._x=x;
                var r=random(ship_h/2);
                mc._y=y_pos+r;
                y_pos+=target_h+(ship_h/2);
            }
            x+=2*target_w;
            i++;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"redTargets");

    //slowArea
    #define min_y height/4
    #define rand_y height/2
    int r=(wdt/2)<(height-min_y)?(height-min_y):(wdt/2);
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u",width);
    actionf_sprite(presprite,buf,"var min_y=%u;var rand_y=%u;var r=%u",min_y,rand_y,r);
    actionf_sprite(presprite,buf,"var green_w=%u;var green_h=%u",green_sz[0],green_sz[1]);
    actionf_sprite(presprite,buf,"var shot_interval=%u",shot_interval);
    action_sprite(presprite,R"(
        var x=width;
        //asta e sa fie repede fogs sub targets
        createEmptyMovieClip('fogs',getNextHighestDepth());
        createEmptyMovieClip('targets',getNextHighestDepth());
        //10% pe glont este descresterea
        var scale_nr=10/(shot_interval/2);
        //
        var i=0;while(i<5){
            var d=targets.getNextHighestDepth();
            var mc=targets.attachMovie('green','green'+d,d);
            x+=r;
            mc._x=x;mc._y=min_y+random(rand_y);
            d=fogs.getNextHighestDepth();
            var mv=fogs.attachMovie('fog','fog'+d,d);mv._x=mc._x+(green_w/2);mv._y=mc._y+(green_h/2);
            mc['fog']=mv;
            mc['hitTest_function']=function(){
                this['fog']['next_scale']=this['fog']._xscale-10;
                if(this['fog']['next_scale']<0){
                    this['fog'].removeMovieClip();
                    this.removeMovieClip();
                }
                else{
                    this['fog'].onEnterFrame=function(){
                        this._xscale-=scale_nr;this._yscale-=scale_nr;
                        if(this._xscale<this.next_scale)this.onEnterFrame=undefined;
                    }
                }
            }
            x+=r;
            i++;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"slowArea");
    //fog
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var r=%u",r);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        onEnterFrame=ep.env_hitTest;
        //
        beginFill(0x00ff00,50);
        //
        var x=0-r;var yFix=0-r;var off=40;var off_in=30;
        var half_off=off/2;
        moveTo(x,yFix);
        x+=off;
        while(x<r){
            curveTo(x-half_off,yFix,x,yFix+off_in);
            x+=off;
        }
        var xFix=r;y=yFix+off;
        while(y<r){
            curveTo(xFix,y-half_off,xFix-off_in,y);
            y+=off;
        }
        yFix=r;x=xFix-off;
        while((0-r)<x){
            curveTo(x+half_off,yFix,x,yFix-off_in);
            x-=off;
        }
        xFix=0-r;y=yFix-off;
        while((0-r)<y){
            curveTo(xFix,y+half_off,xFix+off_in,y);
            y-=off;
        }
        lineTo(xFix,0-r);
        //
        endFill();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"fog");

    //fastArea
    #define fast_w 80
    #define fast_h 40
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var x=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var margin_h=%u",margin_sz[1]);
    actionf_sprite(presprite,buf,"var fast_w=%u;var fast_h=%u",fast_w,fast_h);
    action_sprite(presprite,R"(
        var y=margin_h+random(height-(2*margin_h)-fast_h);
        var on_y=fast_h/2*(random(2)*2-1);
        //margin
        function set_margin(yStep){
            if(yStep<0)return margin_h;
            else return height-margin_h-fast_h;
        }
        var y_margin=set_margin(on_y);
        var i=0;while(i<40){
            var d=getNextHighestDepth();
            var mc=attachMovie('fastRoad','fastRoad'+d,d);mc._x=x;mc._y=y;
            var a=y<y_margin;var b=y+on_y<y_margin;
            if(a!=b){on_y*=-1;y_margin=set_margin(on_y);}
            y+=on_y;
            x+=fast_w;
            i++;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"fastArea");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fast_w=%u;var fast_h=%u",fast_w,fast_h);
    actionf_sprite(presprite,buf,"var fast_env_step=%u",fast_env_step);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var nr=2;
        var arrow_w=fast_w/nr;
        lineStyle(3,0x00FF00,50);
        var x=0;
        while(0<nr){
            moveTo(x,0);
            lineTo(x+arrow_w,fast_h/2);
            lineTo(x,fast_h);
            x+=arrow_w;
            nr--;
        }
        onEnterFrame=function(){
            ep.env_hitTest_fn(this,fast_env_step);
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"fastRoad");

    //winds
    #define wind_w width/2
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var x=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var shield_h=%u",ship_sz[1]+50);
    actionf_sprite(presprite,buf,"var wind_w=%u",wind_w);
    actionf_sprite(presprite,buf,"var margin_h=%u",margin_sz[1]);
	action_sprite(presprite, R"(
        var rand_sz=height-(2*margin_h)-shield_h;rand_sz/=3;
        var y;
        function add_wind(h){
            var d=getNextHighestDepth();
            var mc=attachMovie('wind','wind'+d,d);
            mc._x=x;mc._y=y;mc['h']=h;
        }
        var w_obst=10;var switcher=1;
        var i=0;while(i<5){
            y=0;var rand_y=margin_h+random(rand_sz);
			if(switcher==1)rand_y+=rand_sz+rand_sz;switcher^=1;
            add_wind(rand_y-y);
            y+=rand_y;
            var d=getNextHighestDepth();
            var mc=attachMovie('obst','obst'+d,d);mc['w']=w_obst;
            mc._x=x+wind_w-w_obst;mc._y=y;mc['h']=shield_h;
            y+=shield_h;
            add_wind(height-y);
            x+=wind_w;
            i++;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"winds");
    //wind
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fps=%u",fps);
    actionf_sprite(presprite,buf,"var wind_w=%u",wind_w);
    actionf_sprite(presprite,buf,"var hit_slow=%u",hit_slow);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var dur=0;
        var xSpace=wind_w/2;var rand_sz=wind_w-xSpace;
        //
        var y=0;var inter_min=15;
        var x_rand_nrs=new Array();
        while(y<h){
            x_rand_nrs.push(random(rand_sz));
            y+=inter_min;
        }
        //
        onEnterFrame=function(){
            ep.env_hitTest_fn(this,hit_slow);
            dur--;if(0<dur)return null;dur=fps/2;
            clear();lineStyle(1,0xffFFff,50);
            var y=0;var nr=0;
            while(y<h){
                var x_rand=x_rand_nrs[nr];
                x_rand_nrs[nr]-=xSpace/2;
                if(x_rand_nrs[nr]+xSpace<0)x_rand_nrs[nr]=random(rand_sz);
                nr++;
                //
                moveTo(0,y);
                if(x_rand>0)lineTo(x_rand,y);
                moveTo(x_rand+xSpace,y);lineTo(wind_w,y);
                y+=inter_min+random(10);
            }
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"wind");

    //doors
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var x=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var ship_h=%u",ship_sz[1]);
    actionf_sprite(presprite,buf,"var margin_h=%u",margin_sz[1]);
    //
    actionf_sprite(presprite,buf,"var hit_slow=%u",hit_slow);
    actionf_sprite(presprite,buf,"var door_step=%u",y_step);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var laser_to_door=ship_h;
        var y;
        var door_smallSpace=10;
        //
        var i=random(2);var i_max=i+7;
        while(i<i_max){
            var d=getNextHighestDepth();
            var mc=createEmptyMovieClip('door_laser'+d,d);
            mc.lineStyle(1,0xff0000);
            mc.lineTo(0,height);
            mc.onEnterFrame=function(){
                if(this.hitTest(ep.ship.ship_body)==true){
                    this.top_door.onEnterFrame=top_fn;
                    this.bottom_door.onEnterFrame=bottom_fn;
                    this.removeMovieClip();
                }
            }
            mc._x=x;
            //
            x+=laser_to_door;
            if((i&1)==0)y=margin_h+ship_h;
            else y=height-margin_h-ship_h-door_smallSpace;
            d=getNextHighestDepth();
            var mv=attachMovie('obst','obst'+d,d);mv['no_hit']=true;
            mv._x=x;mv._y=0;mv['h']=y;
            mc['top_door']=mv;
            y+=door_smallSpace;
            d=getNextHighestDepth();
            mv=attachMovie('obst','obst'+d,d);mv['no_hit']=true;
            mv._x=x;mv._y=y;mv['h']=height-y;
            mc['bottom_door']=mv;
            //
            x+=500;
            i++;
        }
        function top_fn(){
            ep.env_hitTest_fn(this,hit_slow);
            this._y-=door_step;
            if((this._y+this.h)<0)this.onEnterFrame=undefined;
        }
        function bottom_fn(){
            ep.env_hitTest_fn(this,hit_slow);
            this._y+=door_step;
            if(height<this._y)this.onEnterFrame=undefined;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"doors");

    //unstableRoad
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u",width);
    action_sprite(presprite,R"(
        var i=0;
        var x=width;
        while(i<10){
            var d=getNextHighestDepth();
            var mc=attachMovie('unst_road','unst_road'+d,d);
            mc['nr']=i&1;
            mc._x=x;
            x+=width;
            i++;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"unstableRoad");
    //
    presprite=swf_sprite_new();
    #define arrow_w 40
    #define arrow_h 40
    #define arrow_step y_step/2
    actionf_sprite(presprite,buf,"var height=%u",height);
    actionf_sprite(presprite,buf,"var ship_h=%u",ship_sz[1]);
    actionf_sprite(presprite,buf,"var arrow_h=%u",arrow_h);
    actionf_sprite(presprite,buf,"var arrow_step=%u",arrow_step);
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var times=1;
        createEmptyMovieClip('arrows',getNextHighestDepth());
        var up_road=arrows.attachMovie('road_arrows','up_road',arrows.getNextHighestDepth());up_road['sense']=-1;
        var down_road=arrows.attachMovie('road_arrows','down_road',arrows.getNextHighestDepth());down_road['sense']=1;
        var half=height/2;
        up_road._y=half;
        down_road._y=half;
        if(nr==0){
			var off=ship_h/3;
			up_road._y-=off;
            down_road._y+=off;
            arrows.onEnterFrame=function(){
                if(this.hitTest(ep.ship.ship_body)==true){
                    var b=ep.ship.ship_body.getBounds(this.down_road);
                    var ship_yCenter=(b.yMax-b.yMin)/2+b.yMin+off;
                    if(ship_yCenter<0)ep.ship._y-=arrow_step;
                    else ep.ship._y+=arrow_step;
                }
            }
        }
        else{
            up_road._y-=ship_h;
            down_road._y+=ship_h;
            attachMovie('dust','dust_center',getNextHighestDepth());
            dust_center['h']=down_road._y-up_road._y;
            dust_center._y=up_road._y;
            times++;
			var htest=function(){
				if(this.hitTest(ep.ship.ship_body)==true)
                    ep.ship._y+=arrow_step*this.sense;
            }
			up_road.onEnterFrame=htest;down_road.onEnterFrame=htest;
        }
        up_road['times']=times;
        down_road['times']=times;
        attachMovie('dust','dust_up',getNextHighestDepth());
        dust_up['h']=up_road._y-(arrow_h*times);
        attachMovie('dust','dust_down',getNextHighestDepth());
        dust_down._y=down_road._y+(arrow_h*times);
        dust_down['h']=height-dust_down._y;
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"unst_road");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u",width);
    actionf_sprite(presprite,buf,"var arrow_w=%u;var arrow_h=%u",arrow_w,arrow_h);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var y=0;
        lineStyle(1,0xffFFff,70);
        while(times>0){
            var x=arrow_w/2*((sense+1)/2);
            while((x+arrow_w)<width){
                moveTo(x,y);
                x+=arrow_w/2;
                lineTo(x,arrow_h*sense+y);
                x+=arrow_w/2;
                lineTo(x,y);
                x+=arrow_w/3;
            }
            y+=arrow_h*sense;
            times--;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"road_arrows");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u",width);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        beginFill(0xffaa77,50);
        lineTo(width,0);lineTo(width,h);
        lineTo(0,h);lineTo(0,0);
        endFill();
        onEnterFrame=ep.env_hitTest;
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"dust");

    //roadPop
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var margin_h=%u;var ship_h=%u",margin_sz[1],ship_sz[1]);
    actionf_sprite(presprite,buf,"var green_h=%u",green_sz[1]);
    action_sprite(presprite,R"(
        createEmptyMovieClip('targets',getNextHighestDepth());
        var rand_sz=height-(2*margin_h)-(2*ship_h);
        var x=width;
        var i=0;while(i<10){
            var d=targets.getNextHighestDepth();
            var mc=targets.attachMovie('green','green'+d,d);
            mc._x=x;mc._y=margin_h+ship_h+random(rand_sz);
            mc['hitTest_function']=function(){
                var d=getNextHighestDepth();
                var mc=attachMovie('road_pop','road_pop'+d,d);
                //se face asa ca daca as da remove aici la ultima nu ar mai astepta zone.removeMovieClip()
                mc['mv_to_delete']=this;
                mc._x=this._x;mc._y=this._y+(green_h/2);
            }
            x+=width;
            i++;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"roadPop");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var in_frames=%u",fps/8);
    actionf_sprite(presprite,buf,"var fast_env_step=%u",fast_env_step);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var arrow_h=40;var arrow_w=20;var arrow_nr=20;
        function clear_the_draw(){
            clear();
            lineStyle(1,0xffFFff);
        }
        //
        mv_to_delete.removeMovieClip();
        //
        var frames_nr=in_frames;var all_w=arrow_w*arrow_nr;
        var in_step=all_w/frames_nr;var dist=0;
        onEnterFrame=function(){
            dist-=in_step;
            clear_the_draw();
            lineTo(dist,0);
            frames_nr--;
            if(frames_nr==0){
                frames_nr=in_frames;
                in_step=(arrow_h/2)/in_frames;dist=0;
                onEnterFrame=function(){
                    dist+=in_step;
                    clear_the_draw();
                    var x=0;
                    var i=0;while(i<arrow_nr){
                        moveTo(x,0);
                        lineTo(x-arrow_w,0-dist);
                        moveTo(x,0);
                        lineTo(x-arrow_w,dist);
                        x-=arrow_w;
                        i++;
                    }
                    frames_nr--;
                    if(frames_nr==0){
                        onEnterFrame=function(){
                            ep.env_hitTest_fn(this,fast_env_step);
                        }
                    }
                }
            }
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"road_pop");

    //
    actionf(buf,"var height=%u",height);
    actionf(buf,"var ship_h=%u",ship_sz[1]);
    actionf(buf,"var fuel_max=%u",fuel_max);
    actionf(buf,"var margin_h=%u",margin_sz[1]);
    action(R"(
        com.space_env(this,'env',getNextHighestDepth());
        attachMovie('margin','up_margin',getNextHighestDepth());up_margin['d']=-1;
        attachMovie('margin','down_margin',getNextHighestDepth());down_margin._y=height-margin_h;down_margin['d']=1;
        var zoneNr=0;
        function new_zone(){
            if(zoneNr==0)attachMovie('one2three','zone',getNextHighestDepth());
            else{
                zone.removeMovieClip();
                if(zoneNr==1)attachMovie('simpleObstacles','zone',getNextHighestDepth());
                else if(zoneNr==2)attachMovie('holes','zone',getNextHighestDepth());
                else if(zoneNr==3)attachMovie('redTargets','zone',getNextHighestDepth());
                else if(zoneNr==4)attachMovie('slowArea','zone',getNextHighestDepth());
                else if(zoneNr==5)attachMovie('fastArea','zone',getNextHighestDepth());
                else if(zoneNr==6)attachMovie('winds','zone',getNextHighestDepth());
                else if(zoneNr==7)attachMovie('doors','zone',getNextHighestDepth());
                else if(zoneNr==8)attachMovie('unstableRoad','zone',getNextHighestDepth());
                else if(zoneNr==9)attachMovie('roadPop','zone',getNextHighestDepth());
                else _root.end_scenario();
            }
            zone.onEnterFrame=function(){
                var r=this.getBounds(ep);
                if(r.xMax<0)new_zone();
            }
            zoneNr++;
        }
        new_zone();
        attachMovie('ship','ship',getNextHighestDepth());
        createEmptyMovieClip('bullets',getNextHighestDepth());
        ship._x=fuel_max;ship._y=(height-ship_h)/2;
        function env_x_move(value){
            up_margin.draw_margin(value);
            down_margin.draw_margin(value);
            zone._x+=value;
        }
    )");
    actionf(buf,"var hit_slow=%u",hit_slow);
    action(R"(
        function env_hitTest(){
            env_hitTest_fn(this,hit_slow);
        }
        function env_hitTest_fn(mc,value){
            if(mc.hitTest(ship.ship_body)==true)env_x_move(value);
        }
    )");
	action(R"(
        function bullet_action(mc){
            var ts=zone.targets;
            for(var mc_name in ts){
                if(mc.hitTest(ts[mc_name])==true){
                    ts[mc_name].hitTest_function();
                    mc.removeMovieClip();
                    break;
                }
            }
        }
    )");
	/*_root.qwert.text = b.yMax + ' ' + b.yMin;
	action(R"(
	var asdf=new TextFormat();
	asdf.color=0x00ff00;asdf.size=50;
	_root.createTextField('qwert',_root.getNextHighestDepth(),0,0,100,50);
	_root.qwert.setNewTextFormat(asdf);
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
