

#include<windows.h>
#include<stdio.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

int _start(){
    char* ep_nm=Rocks_Fall_ep;
	sprintf(buf, "%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);
    init_episod(ep_nm);
    action("_root['episode']=this");

    //roata
    int wheel_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/4/roata.dbl",wheel_sz);
    swf_exports_add(dbl,"wheel");

    //smoke
    int smoke_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/4/smoke1.dbl",smoke_sz);
    swf_sprite_placeobject(presprite,bmp,0);for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    bmp=swf_image("C:/Desktop/universe/img/4/smoke2.dbl");
    swf_sprite_removeobject(presprite,0);
    swf_sprite_placeobject(presprite,bmp,0);for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    bmp=swf_image("C:/Desktop/universe/img/4/smoke3.dbl");
    swf_sprite_removeobject(presprite,0);
    swf_sprite_placeobject(presprite,bmp,0);for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    bmp=swf_image("C:/Desktop/universe/img/4/smoke4.dbl");
    swf_sprite_removeobject(presprite,0);
    swf_sprite_placeobject(presprite,bmp,0);for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    bmp=swf_image("C:/Desktop/universe/img/4/smoke5.dbl");
    swf_sprite_removeobject(presprite,0);
    swf_sprite_placeobject(presprite,bmp,0);for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"smoke");

    //gun
        //support
    int gun_support_sz[2];
    presprite=swf_sprite_new();bmp=swf_image_ex("C:/Desktop/universe/img/4/support.dbl",gun_support_sz);swf_sprite_placeobject(presprite,bmp,0);sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"gun_support");
        //tube
    int gun_tube_sz[2];
    presprite=swf_sprite_new();bmp=swf_image_ex("C:/Desktop/universe/img/4/tube.dbl",gun_tube_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0,0-(gun_tube_sz[1]/2));
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        ep['gun']=this;
        var tube_point=new Object();tube_point.x=0;tube_point.y=0;this.localToGlobal(tube_point);
        tube_point.x-=ep._x;tube_point.y-=ep._y;
        ep.bullets._x=tube_point.x;ep.bullets._y=tube_point.y;
        ep.ramps_set(tube_point.y);
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"gun_tube");
        //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var gun_support_w=%u;var gun_tube_h=%u",gun_support_sz[0],gun_tube_sz[1]);
    action_sprite(presprite,R"(
        attachMovie('gun_support','support',0);
        attachMovie('gun_tube','tube',1);
        tube._x=gun_support_w/2;tube._y=gun_tube_h/2;
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"car_gun");

    //car
    int car_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/4/car.dbl",car_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    actionf_sprite(presprite,buf,"var wheel_w=%u;var wheel_h=%u;var gun_support_h=%u",wheel_sz[0],wheel_sz[1],gun_support_sz[1]);
    actionf_sprite(presprite,buf,"var car_w=%u;var car_h=%u",car_sz[0],car_sz[1]);
    action_sprite(presprite,R"(
        var main=_root['episode'];
        var com=_root['shared_level'];
        createEmptyMovieClip('wheel_A',1);
        com.mc_rotation_bmp_show(flash.display.BitmapData.loadBitmap('wheel'),wheel_A,'wheelBack',0,main.rot_step);
        main.wheelA=wheel_A.wheelBack;
        createEmptyMovieClip('wheel_B',2);
        com.mc_rotation_bmp_show(flash.display.BitmapData.loadBitmap('wheel'),wheel_B,'wheelFront',0,main.rot_step);
        main.wheelB=wheel_B.wheelFront;
        wheel_A._x=1;wheel_A._y=car_h-wheel_h;
        wheel_B._x=car_w-1-wheel_w;wheel_B._y=wheel_A._y;
        //
        attachMovie('car_gun','gun',3);gun._x=3;gun._y=0-gun_support_h;
    )"); swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"car_body");
    //
    actionf(buf,"var car_w=%u",car_sz[0]);

    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var smoke_w=%u;var car_h=%u",smoke_sz[0],car_sz[1]);
    action_sprite(presprite,R"(
        attachMovie('smoke','smoke',0);
        smoke._x=0-smoke_w;smoke._y=car_h/2;smoke._alpha=80;
        attachMovie('car_body','body',1);
        _root['episode'].car=body;
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"car_mc");

	automatic(3);
	action(R"(
    function car_control(){
        //rotarile cad pietre si in spatele tubului, e ok -180, si mai sunt 90, le las si pe-astea,360
        if(Key.isDown(com.up_arrow)==true){
            gun._rotation-=gun_step;
        }
        else if(Key.isDown(com.down_arrow)==true){
            gun._rotation+=gun_step;
        }
        else if(Key.isDown(com.jump_aux_key)==true){
            gun._rotation=0;
        }
        if(Key.isDown(com.action_key)==true)shot_launch();
		else if(auto_shot==true)shot_launch();
        //
        if(Key.isDown(com.left_arrow)==true){
            zone._x+=step;
            if(zone._x>0)zone._x=0;
            else{
                sky._x+=sky_step;if(sky._x>0)sky._x-=width;
                background._x+=step;
            }
            com.rotate_mc_andChangeStep(wheelA,0-rot_step);com.rotate_mc_andChangeStep(wheelB,0-rot_step);
        }
        else if(Key.isDown(com.right_arrow)==true)forward();
		else if(auto_move==true)forward();
    }
	function forward(){
		if(car.hit!=undefined)return undefined;
        if(env_test()==true)return undefined;
		zone._x-=step;
        sky._x-=sky_step;if(sky._x<(0-width))sky._x+=width;
        background._x-=step;
        if(zone._x+zone_x_max<width){inter_ground_set();new_zone();}
        com.rotate_mc_andChangeStep(wheelA,rot_step);com.rotate_mc_andChangeStep(wheelB,rot_step);
	}
    //bool
    function env_test(){
        car.bounds_obj=car.getBounds(_root);
        for(var obj in zone.targets)if(car_test(zone.targets[obj])==true)return env_test_object_hit(zone.targets[obj]);
        for(var obj in zone.impediments)if(car_test(zone.impediments[obj])==true)return env_test_object_hit(zone.impediments[obj]);
        return false;
    }
    //bool
    function env_test_object_hit(object){
        if(object.envTest_function==undefined)return true;
        return object.envTest_function();
    }

    //bool
    function car_test(mc){
        var bounds_obj=car.bounds_obj;
        var bottom=bounds_obj.yMax;
        if(mc.allow_top==true)bottom--;
        //
        var mc_bounds=mc.getBounds(_root);
        if(overlap_test(bounds_obj.xMin,bounds_obj.xMax,mc_bounds.xMin,mc_bounds.xMax)==false)return false;
        return overlap_test(bounds_obj.yMin,bottom,mc_bounds.yMin,mc_bounds.yMax);
    }
    //bool
    function overlap_test(pA1,pA2,pB1,pB2){
        //aicea e sau egal ca altfel vine car jos minus allow_top==bridge_sus
        if(pA2<=pB1)return false;
        //if(pB1<pA1)??????????
        if(pB2<pA1)return false;
        return true;
    }
    )");

    //
    //float step=ship_sz[0];float part=fps;part/=2;step/=part;
    actionf(buf,"var step=%u",car_sz[0]*2/fps);
    actionf(buf,"var gun_step=%u",45/fps);
    actionf(buf,"var width=%u;var height=%u;var car_h=%u",width,height,car_sz[1]);
    action("var land_y=height*4/5;var land_to_height=height-land_y");
    actionf(buf,"var gun_tube_w=%u;var fps=%u",gun_tube_sz[0],fps);
    int rot_step=90/fps;
    actionf(buf,"var rot_step=%u",rot_step);
    action("var sky_step=step/4");

    int ground_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/4/ground.dbl",ground_sz);
    swf_exports_add(dbl,"ground");

    //shots
    actionf(buf,"var shot_step_factor=24/%u",fps);
	actionf(buf, "var shot_inter_time=%u", shot_interval);
	action(R"(
    var shot_size=10;
    function shot_launch(){
		if(shot_timer==undefined){
			createEmptyMovieClip('shot_timer',getNextHighestDepth());
			shot_timer['interval']=shot_inter_time;
			shot_timer.onEnterFrame=function(){
				this.interval--;if(this.interval<0)this.removeMovieClip();
			}
			//
			var d=bullets.getNextHighestDepth();
			var bullet_name='bullet'+d;
			bullets.createEmptyMovieClip(bullet_name,d);
			var bullet=bullets[bullet_name];
			var degrees=gun._rotation;
			bullet.lineStyle(2,0xff0000);bullet.moveTo(gun_tube_w,0);bullet.lineTo(gun_tube_w+shot_size,0);bullet._rotation=degrees;
			var x_line=Math.cos(degrees*Math.PI/180)*shot_size;bullet['x_step']=x_line*shot_step_factor;
			var y_line=Math.sin(degrees*Math.PI/180)*shot_size;bullet['y_step']=y_line*shot_step_factor;
			bullet.onEnterFrame=bullet_move;
		}
    }
    function outOfScreen(rect){
	if(rect.xMin>width)return true;
	if(rect.yMin>height)return true;
	if(rect.xMax<0)return true;
	if(rect.yMax<0)return true;
	return false;
    }
    function bullet_move(){
	var test=outOfScreen(this.getBounds(_root['episode']));
	if(test==true){
	    this.removeMovieClip();
	}
        else{
            this._x+=this.x_step;
            this._y+=this.y_step;

            var b_test=this.getBounds(_root);
            var x=(b_test.xMin+b_test.xMax)/2;
            var y=(b_test.yMin+b_test.yMax)/2;

            //nu s-a testat hitTestul cu groundul si nu il pun, ca e mai rapid asa

            for(mc_name in zone.targets){
                var mc=zone.targets[mc_name];
                if(mc.hitTest(x,y,true)==true){mc.onEnterFrame=mc['hitTest_function'];this.removeMovieClip();break;}
            }

            //astea-s doar la obstacole
            if(zone.ramps.hitTest(x,y,true)==true)this.removeMovieClip();
            for(mc_name in zone.impediments)if(zone.impediments[mc_name].hitTest(x,y,true)==true)this.removeMovieClip();
        }
    }
    )");

    action("var matrix=new flash.geom.Matrix();var BitmapData=flash.display.BitmapData");

    //rock
    int rock_sz[2];int ramp_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/4/rock.dbl",rock_sz);
    swf_exports_add(dbl,"rock_main");

    //
    action(R"(
    function rock_at_ramp(rock){
        if(rock.hittest_value==0){
            rock.rock_main.rotate_bmp();
            var w_2=rock_w/2;
            var x_point=_x+zone._x+rock._x+w_2;var y_point=_y+zone._y+rock._y+rock_h;
            if(rock.hittest_mc.hitTest(x_point+w_2,y_point,true)==true)rock.hittest_value=1;
            else if(rock.hittest_mc.hitTest(x_point-w_2,y_point,true)==true)rock.hittest_value=1;
        }
        if(rock.hittest_value==0){
            if(rock.ramp.bottom_ramp.hitTest(x_point,y_point,true)==true)if(rock.x_step>0){rock.x_step*=-1;rock.rock_main.rot*=-1;}
            //
            if(rock.ramp.hitTest(x_point,y_point,true)==true)rock._x+=rock.x_step;
            else rock._y+=rock.y_step;
        }
    }

    )");
    //rock_bridge
    presprite=swf_sprite_new();
    //
    actionf_sprite(presprite,buf,"var rock_w=%u;var rock_h=%u",rock_sz[0],rock_sz[1]);
    action_sprite(presprite,R"(
    function hitTest_function_rampAndBridge(){
        var ep=_root['episode'];
        ep.rock_at_ramp(this);
        if(hittest_value==1){
            hittest_value=2;
            hittest_mc.onEnterFrame=hittest_mc.hitTest_function;
            distBridge=hittest_mc._y-_y-rock_h;
        }
        else if(hittest_value==2){
            rock_main.rotate_bmp();
            //smecherie ca sa cada mai drept
            distBridge-=distBridge/50;
            //
            var x=Math.sin(hittest_mc._rotation*Math.PI/180)*distBridge;
            var y=Math.cos(hittest_mc._rotation*Math.PI/180)*distBridge;
            y=hittest_mc._y-y;
            _y=y-rock_h;
            _x=hittest_mc._x+x;
            if(x_step>0)_x-=rock_w+(rock_w/2);
            else _x+=rock_w/2;
            if(hittest_mc.at_ground==true){
                _y=hittest_mc._y-ep.bridge_half_w-rock_h;
                rock_main.removeMovieClip();
                //
                onEnterFrame=undefined;
                hitTest_function=hitTest_function_resist;
                //
                hole_mc['allow_top']=true;
                //
                gotoAndStop(2);
            }
        }
    }
    var hitTest_function=hitTest_function_rampAndBridge;
    stop();
    function hitTest_function_resist(){
        removeMovieClip();
    }
    )");
    swf_sprite_showframe(presprite);
    //
    bmp=swf_image("C:/Desktop/universe/img/4/rock_damaged.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    swf_sprite_showframe(presprite);
    //
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"rock_bridge");

    //rock_fill
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var rock_w=%u;var rock_h=%u",rock_sz[0],rock_sz[1]);
    action_sprite(presprite,R"(
        function hitTest_function_rampAndFill(){
            var ep=_root['episode'];
            ep.rock_at_ramp(this);
            if(hittest_value==1){
                _x=x_fix;ramp.breaker.removeMovieClip();hittest_value=2;
            }
            else if(hittest_value==2){
                _y+=y_step;
                //ok =
                if(_y>=ep.land_y){
                    _y=ep.land_y;
                    rock_main.removeMovieClip();
                    //
                    onEnterFrame=undefined;
                    hitTest_function=undefined;
                    //
                    gotoAndStop(2);
                }
            }
        }
        var hitTest_function=hitTest_function_rampAndFill;
        stop();
    )");swf_sprite_showframe(presprite);
    //
    int rock_fill_sz[2];
    bmp=swf_image_ex("C:/Desktop/universe/img/4/rock_fill.dbl",rock_fill_sz);
    actionf(buf,"var rock_fill_h=%u",rock_fill_sz[1]);
    swf_sprite_placeobject(presprite,bmp,0);
    swf_sprite_showframe(presprite);
    //
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"rock_fill");

    //ramp
    presprite=swf_sprite_new();
    dbl=swf_dbl_ex("C:/Desktop/universe/img/4/panta.dbl",ramp_sz);
    swf_exports_add(dbl,"ramp");

    action("var ramps_bottom");
    actionf(buf,"var ramp_w=%u;var ramp_h=%u;var rock_w=%u;var rock_h=%u",ramp_sz[0],ramp_sz[1],rock_sz[0],rock_sz[1]);
    action(R"(
    var ramps_rand_bottom;var rampsDouble_rand_bottom;
    function ramps_set(tube_point_y){
        ramps_bottom=tube_point_y-gun_tube_w-5;
        ramps_rand_bottom=ramps_bottom-ramp_h-rock_h;
        rampsDouble_rand_bottom=ramps_rand_bottom-ramp_h;
        zone_proc(0,false);
        new_zone();
    }

    var ramp_bmp=BitmapData.loadBitmap('ramp');var second_ramp_offset=rock_w*3/2;
    var y_ramp;
    function get_y_ramp(ramp_is_double){
        if(ramp_is_double!=true)y_ramp=random(ramps_rand_bottom);
        else y_ramp=random(rampsDouble_rand_bottom);
        y_ramp+=rock_h;
    }
    //ramp
    function add_ramp(is_double,rock_type){
        var c=zone['ramps'];
        var d=c.getNextHighestDepth();
        var ramp=c.createEmptyMovieClip('ramp'+d,d);
        ramp._x=zone_x;ramp._y=y_ramp;

        var top_ramp=ramp.createEmptyMovieClip('top_ramp',breaker_depth+1);
        top_ramp.beginBitmapFill(ramp_bmp,matrix,false);
        top_ramp.lineTo(rock_w/2,0);top_ramp.lineTo(ramp_w,ramp_h);
        top_ramp.lineTo(0,ramp_h);top_ramp.lineTo(0,0);
        top_ramp.endFill();
        //
        if(is_double==true){
            var secRamp=ramp.createEmptyMovieClip('bottom_ramp',breaker_depth+2);secRamp._x=second_ramp_offset;secRamp._y=ramp_h;
            secRamp.beginBitmapFill(ramp_bmp,matrix,false);secRamp,moveTo(ramp_w,0);
            secRamp.lineTo(ramp_w-(rock_w/2),0);secRamp.lineTo(0,ramp_h);
            secRamp.lineTo(ramp_w,ramp_h);secRamp.lineTo(ramp_w,0);
            secRamp.endFill();
        }
        var c=zone['targets'];
        var d=c.getNextHighestDepth();
        var rock=c.attachMovie(rock_type,rock_type+d,d);
        rock._x=zone_x;rock._y=y_ramp-rock_h;rock['ramp']=ramp;
        rock['y_step']=step;rock['x_step']=step;
        rock['hittest_value']=0;
        //
        com.mc_rotation_bmp_show(flash.display.BitmapData.loadBitmap('rock_main'),rock,'rock_main',rock.getNextHighestDepth(),rot_step);
        //
        ramp['rock']=rock;
        return ramp;
    }
    var breaker_depth=0;
    //ramp
    function add_ramp_stop(ramp_is_double){
        get_y_ramp(ramp_is_double);
        var ramp=add_ramp(ramp_is_double,'rock_fill');
        //
        var nx_dp=ramp.getNextHighestDepth();
        var stop=ramp.attachMovie('stoper','stoper',nx_dp);
        //
        ramp.rock['hittest_mc']=stop;ramp.rock['allow_top']=true;
        //
        var breaker=ramp.createEmptyMovieClip('breaker',breaker_depth);
        var line_sz=ramp.rock.y_step+1;
        breaker.beginBitmapFill(ramp_bmp,matrix,true);
        var rock_to_stop_at_x;
        if(ramp_is_double!=true){
            stop._x=ramp_w+rock_w;stop._y=ramp_h-stoper_h;
            breaker._y=ramp_h-(line_sz/2);
            var breaker_left=line_sz;var breaker_right=stop._x;
            rock_to_stop_at_x=stop._x-rock_w;
        }
        else{
            stop._x=ramp.bottom_ramp._x-rock_w-stoper_w;stop._y=ramp.bottom_ramp._y+ramp_h-stoper_h;
            breaker._y=stop._y+stoper_h-(line_sz/2);
            var breaker_left=stop._x+stoper_w;var breaker_right=ramp.bottom_ramp._x+ramp_w-line_sz;
            rock_to_stop_at_x=stop._x+stoper_w;
        }
        breaker.moveTo(breaker_left,0);
        breaker.lineTo(breaker_left,0-(line_sz/2));breaker.lineTo(breaker_right,0-(line_sz/2));
        breaker.lineTo(breaker_right,line_sz/2);breaker.lineTo(breaker_left,line_sz/2);
        breaker.endFill();
        ramp.rock['x_fix']=ramp._x+rock_to_stop_at_x;
        return ramp;
    }
    )");

    int stoper_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/4/stop.dbl",stoper_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"stoper");
    actionf(buf,"var stoper_w=%u;var stoper_h=%u",stoper_sz[0],stoper_sz[1]);

    //bridge
    int bridge_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/4/bridge.dbl",bridge_sz);
    swf_exports_add(dbl,"bridge");
    actionf(buf,"var bridge_w=%u;var bridge_h=%u",bridge_sz[0],bridge_sz[1]);
    actionf(buf,"var bridge_step=%u",45/fps);
    action(R"(
    var bridge_half_w=bridge_w/2;var bridge_bottom=land_y+bridge_half_w;
    var bridge_bmp=BitmapData.loadBitmap('bridge');var current_bridge_h;
    function bridge_init(ramp_is_double){
        get_y_ramp(ramp_is_double);current_bridge_h=bridge_bottom-y_ramp;
        if(ramp_is_double==true)current_bridge_h-=ramp_h;
        next_zone_x_max=zone_x+ramp_w+bridge_half_w+current_bridge_h+bridge_half_w;
        //in dreapta e la fel:=zone_x+bridge_half_w+current_bridge_h+bridge_half_w+ramp_w;
    }
    function add_bridge(ramp_is_double){
        zone_x_max=next_zone_x_max;
        var ramp=add_ramp(ramp_is_double,'rock_bridge');
        //
        var c=zone['impediments'];
        var d=c.getNextHighestDepth();
        var hole=c.createEmptyMovieClip('mc'+d,d);
        d=c.getNextHighestDepth();
        var bridge=c.createEmptyMovieClip('bridge'+d,d);
        bridge['allow_top']=true;
        ramp.rock['hittest_mc']=bridge;
        //
        if(ramp_is_double!=true){bridge['step']=bridge_step;bridge['angle']=90;
            bridge._x=ramp._x+ramp_w+bridge_half_w;bridge._y=ramp._y;}
        else{ramp._x=zone_x_max-second_ramp_offset-ramp_w;ramp.rock._x=ramp._x;
            bridge['step']=0-bridge_step;bridge['angle']=-90;
            bridge._x=ramp._x-bridge_half_w+second_ramp_offset;bridge._y=ramp._y+ramp_h;
        }
        //
        var value_x;
        bridge._y+=current_bridge_h;
        //
        var value_x;
        if(ramp_is_double!=true)value_x=0;
        else value_x=0-current_bridge_h;
        ramp.rock['hole_mc']=hole;
        hole._x=bridge._x;hole._y=zone.ground._y;
        hole.moveTo(value_x,0);
        hole.beginFill(0,100);var x_go;var y_go;
        x_go=value_x+current_bridge_h;hole.lineTo(x_go,0);
        y_go=0+bridge_w;hole.lineTo(x_go,y_go);
        x_go-=bridge_w;hole.lineTo(x_go,y_go);
        y_go=land_to_height;hole.lineTo(x_go,y_go);
        x_go=value_x+bridge_w;hole.lineTo(x_go,y_go);
        y_go=0+bridge_w;hole.lineTo(x_go,y_go);
        x_go=value_x;hole.lineTo(x_go,y_go);
        hole.lineTo(value_x,0);
        hole.endFill();
        //
        var br=bridge.createEmptyMovieClip('innerGate',0);br._x=0-bridge_half_w;
        br.beginBitmapFill(bridge_bmp,matrix,true);
        br.lineTo(bridge_w,0);br.lineTo(bridge_w,current_bridge_h);br.lineTo(0,current_bridge_h);br.lineTo(0,0);
        br.endFill();
        br._y-=current_bridge_h;
        //
        bridge.hitTest_function=function(){
            this._rotation+=this.step;
            var boolean_value;
            //e 45/fps 3(fps 12) sau 2(fps 16)
            if(this.step>0)boolean_value=this._rotation>this.angle;
            else boolean_value=this._rotation<this.angle;
            if(boolean_value){
                this._rotation=this.angle;
                this['at_ground']=true;
                this.onEnterFrame=undefined;
            }
        }
    }
    )");

    int hit_anim;int hit_anim_sz[2];

    //zone

    presprite=swf_sprite_new();
    hit_anim=swf_image_ex("C:/Desktop/universe/img/4/hit1.dbl",hit_anim_sz);
    swf_sprite_placeobject(presprite,hit_anim,0);
    action_sprite(presprite,"_alpha=40");
    for(int i=0;i<(fps*2);i++)swf_sprite_showframe(presprite);
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/4/hit2.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<(fps*2);i++)swf_sprite_showframe(presprite);
    action_sprite(presprite,"removeMovieClip()");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"hit");

    //fallers
    int faller_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/4/stone.dbl",faller_sz);
    swf_exports_add(dbl,"stone_bmp");
    dbl=swf_dbl("C:/Desktop/universe/img/4/ice.dbl");
    swf_exports_add(dbl,"ice_bmp");
    //
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        com.mc_rotation_bmp(flash.display.BitmapData.loadBitmap(type+'_bmp'),this,'item',0,0-ep.rot_step);
        this.item.onEnterFrame=this.item.rotate_bmp;
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);
    swf_exports_add(sprite,"nature_object");

    actionf(buf,"var nature_object_w=%u;var nature_object_h=%u",faller_sz[0],faller_sz[1]);
	action(R"(
        var nature_objects_types=new Array('stone','ice');
        var zone_default_width=20*fps*step;var fall_step=step*3/2;
        var stone_interval=fps/2;var stone_interval_cursor=0;
        function fall_init(type){
            zone.onEnterFrame=fall_proc;zone_x_max=zone_x+zone_default_width;
            zone['type']=type;
        }
        function fall_proc(){
            //ok =
            stone_interval_cursor++;if(stone_interval_cursor>=stone_interval)stone_interval_cursor=0;
            else return null;
            var mc=fall_set(-1);
            var rand=random(zone_default_width);
            mc._x=zone_x+rand;
            if(this.type!=undefined){
                mc['type']=this.type;
                mc._y=0-nature_object_h;
            }
            else{
                mc['type']=nature_objects_types[random(nature_objects_types.length)];
                var scale=2;
                mc._y=0-(nature_object_h*scale);
                mc._xscale=100*scale;mc._yscale=mc._xscale;
                mc['hitTest_function']=hitTest_function_large_fallers;
            }
        }
        //mc
        function fall_set(x_mult){
            var z=zone.targets;
            var d=z.getNextHighestDepth();
            var mc=z.attachMovie('nature_object','nature_object'+d,d);
            mc['keep_for_ground_contact']=true;
            //sa cada vertical sau 45 spre stanga
            var angle=random(45);
            mc['x_step']=Math.sin(angle*Math.PI/180)*fall_step*x_mult;
            mc['y_step']=Math.cos(angle*Math.PI/180)*fall_step;
            mc['hitTest_function']=hitTest_function_fallers;
            mc.onEnterFrame=nature_object_move;
            return mc;
        }
        function hitTest_function_fallers(){
            this.removeMovieClip();
        }
        function nature_object_move(){
            this._x+=this.x_step;this._y+=this.y_step;
			if(this._y>=land_y)this.removeMovieClip();
            else if(this.hitTest(car)==true){
                if(car.hit!=undefined)car.hit.removeMovieClip();
                car.attachMovie('hit','hit',car.getNextHighestDepth());
				this.hitTest_function();
            }
        }
        //
        function hitTest_function_large_fallers(){
            var wd=nature_object_w*this._xscale/100;var hg=nature_object_h*this._yscale/100;
            var y_line=(hg-nature_object_h)/2;var x_pos=(wd-(2*nature_object_w))/2;
            var xPos=this._x;var yPos=this._y;
            //
            var mc_left=fall_set(-1);
            mc_left._x=xPos+x_pos;mc_left._y=yPos+y_line;mc_left.type=this.type;
            //
            x_pos+=nature_object_w;
            var mc_right=fall_set(1);
            mc_right._x=xPos+x_pos;mc_right._y=yPos+y_line;mc_right.type=this.type;
            //
            this.removeMovieClip();
        }
    )");

    //obstacles with stoper
    action(R"(
    var lava_step=48/fps;
    //obst
    function obstacle_with_stoper(ramp_is_double){
        zone_x_max=next_zone_x_max;
        var c=zone['impediments'];
        var obst=c.createEmptyMovieClip('obstacle',c.getNextHighestDepth());
        //
        var ramp=add_ramp_stop(ramp_is_double);
        //
        obst._y=ramp._y+ramp_h;
        if(ramp_is_double!=true)obst._x=ramp._x+ramp.stoper._x-vObst_w;
        else{obst._x=ramp._x+ramp.stoper._x+stoper_w;obst._y+=ramp_h;}
        obst['rock']=ramp.rock;
        return obst;
    }
    function next_x_max_stoper(ramp_is_double){
        if(ramp_is_double!=true)next_zone_x_max=zone_x+ramp_w+rock_w+stoper_w;
        else next_zone_x_max=zone_x+ramp_w+second_ramp_offset;
    }
    )");

    //obstacol vertical

    dbl=swf_dbl("C:/Desktop/universe/img/4/volcano.dbl");
    swf_exports_add(dbl,"volcano");
    //
    dbl=swf_dbl("C:/Desktop/universe/img/4/water.dbl");
    swf_exports_add(dbl,"water");

    action(R"(
        var vObst_top_dist;var vObst_h;
        var vObst_w=rock_w;var vObst_random_nr=2;var vObst_step_main=64/fps;
        //obst
        function vertical_obstacle_init(ramp_is_double){
            var obst=obstacle_with_stoper(ramp_is_double);
            vObst_top_dist=land_y-obst._y-car_h;
            vObst_h=height-obst._y;
            return obst;
        }
        function vertical_obstacle_set(obst){
            obst.obst_yPos=vObst_top_dist;
            obst.obst_step=0;
            obst.obst_matrix=new flash.geom.Matrix();
            //yPos
            obst.vertical_obstacle_atFrame=function(){
                this.clear();
                this.obst_yPos+=this.obst_step;
                if(this.obst_yPos<0)this.obst_step=vObst_step_main+random(vObst_random_nr+2);
                //ok =
                else if(this.obst_yPos>=vObst_top_dist)this.obst_step=-1*(vObst_step_main+random(vObst_random_nr));
                //
                var yPos=this.obst_yPos;var yPos_rock_dependent=this.rock._y+(rock_h/2)-this.obst._y;
                if(yPos_rock_dependent>yPos)yPos=yPos_rock_dependent;
                return yPos;
            }
        }
        //
        var volcano_bmp=BitmapData.loadBitmap('volcano');
        function add_volcano(ramp_is_double){
            var volc=vertical_obstacle_init(ramp_is_double);
            var flames_nr=3;
            var volcano_x_off=vObst_w/flames_nr;
            var volcano_line=vObst_top_dist+5;
            var i=0;while(i<flames_nr){
                var v_flame=volc.createEmptyMovieClip('volcano'+i,i);
                v_flame._x=i*volcano_x_off;
                vertical_obstacle_set(v_flame);
                v_flame.lava_step_i=lava_step;
                v_flame.rock=volc.rock;
                v_flame.obst=volc;
                v_flame.onEnterFrame=function(){
                    this.lava_step_i++;if(this.lava_step_i>lava_step){
                        this.lava_step_i=0;
                        this.obst_matrix.ty=random(vObst_h);
                    }
                    var yPos=this.vertical_obstacle_atFrame();
                    if(yPos>volcano_line)volcano_line=yPos;
                    this.beginBitmapFill(volcano_bmp,this.obst_matrix,true);
                    this.moveTo(0,volcano_line);
                    this.curveTo(0,yPos,volcano_x_off/2,yPos);
                    this.curveTo(volcano_x_off,yPos,volcano_x_off,volcano_line);
                    this.lineTo(volcano_x_off,vObst_h);this.lineTo(0,vObst_h);this.lineTo(0,volcano_line);
                    this.endFill();
                }
                i++;
            }
        }
        //
        var water_bmp=BitmapData.loadBitmap('water');
        function add_water(ramp_is_double){
            var water=vertical_obstacle_init(ramp_is_double);
            var water_step=16/fps;
            var water_x_off=vObst_w/3;var water_jet_w=water_x_off-3;
            var water_top_off=2*water_jet_w;
            var water_texture_h=water_bmp.height;
            var water_obst_matrix_sides=new flash.geom.Matrix();
            //
            vertical_obstacle_set(water);
            //e bazinul si trebuie asta
            water['allow_top']=true;
            water['obst']=water;
            this.obst_matrix.ty=water_texture_h;
            water.onEnterFrame=function(){
                this.obst_matrix.ty-=water_step;if(this.obst_matrix.ty<0)this.obst_matrix.ty+=water_texture_h;
                water_obst_matrix_sides.ty+=water_step;if(water_obst_matrix_sides.ty>water_texture_h)water_obst_matrix_sides.ty-=water_texture_h;
                //
                var yPos=this.vertical_obstacle_atFrame();
                var water_yPos_downSide=yPos+water_top_off;
                //
                this.beginBitmapFill(water_bmp,water_obst_matrix_sides,true);
                var off=land_y-this._y;
                this.moveTo(0,off);
                this.lineTo(vObst_w,off);this.lineTo(vObst_w,off+rock_fill_h);
                this.lineTo(0,off+rock_fill_h);this.lineTo(0,off);
                this.endFill();
                //
                this.beginBitmapFill(water_bmp,water_obst_matrix_sides,true);
                this.moveTo(0,water_yPos_downSide);
                this.lineTo(0,vObst_h);this.lineTo(water_jet_w,vObst_h);this.lineTo(water_jet_w,water_yPos_downSide);
                var center_left=vObst_w/2-(water_jet_w/2);
                this.curveTo(vObst_w/4,yPos+water_jet_w,center_left,water_yPos_downSide);
                this.lineTo(vObst_w/2,water_yPos_downSide);
                this.curveTo(vObst_w/4,yPos,0,water_yPos_downSide);
                this.endFill();
                //
                this.beginBitmapFill(water_bmp,this.obst_matrix,true);
                this.moveTo(center_left,water_yPos_downSide);
                var center_right=center_left+water_jet_w;
                this.lineTo(center_left,vObst_h);this.lineTo(center_right,vObst_h);
                this.lineTo(center_right,water_yPos_downSide);this.lineTo(center_left,water_yPos_downSide);
                this.endFill();
                //
                this.beginBitmapFill(water_bmp,water_obst_matrix_sides,true);
                this.moveTo(vObst_w,water_yPos_downSide);
                this.lineTo(vObst_w,vObst_h);this.lineTo(vObst_w-water_jet_w,vObst_h);this.lineTo(vObst_w-water_jet_w,water_yPos_downSide);
                this.curveTo(vObst_w-(vObst_w/4),yPos+water_jet_w,center_right,water_yPos_downSide);
                this.lineTo(vObst_w/2,water_yPos_downSide);
                this.curveTo(vObst_w-(vObst_w/4),yPos,vObst_w,water_yPos_downSide);
                this.endFill();
            }
        }
    )");

    //lava
    action(R"(
    var lava_w=rock_w;
    function add_lava(ramp_is_double){
        var obst=obstacle_with_stoper(ramp_is_double);
        obst._y=land_y;
        obst.lava_matrix=new flash.geom.Matrix();
        obst.lava_step_i=lava_step;
        obst['allow_top']=true;
        obst.createEmptyMovieClip('bubbles',obst.getNextHighestDepth());
        obst.onEnterFrame=function(){
            if(this.rock._y<this._y){
                var d=this.bubbles.getNextHighestDepth();
                var bubble=this.bubbles.createEmptyMovieClip('lava_bubble'+d,d);
                bubble._x=random(lava_w);
                bubble.raiser=0;
                bubble.onEnterFrame=function(){
                    bubble.raiser++;
                    if(bubble.raiser>(lava_w/4))this.removeMovieClip();
                    if(this._x-bubble.raiser<0)this.removeMovieClip();
                    //ok =
                    if(this._x+bubble.raiser>=lava_w)this.removeMovieClip();
                    this.beginFill(0xffFF00,60);
                    this.moveTo(0-bubble.raiser,0);
                    this.curveTo(0,0-bubble.raiser,bubble.raiser,0);
                    this.lineTo(0-bubble.raiser,0);
                    this.endFill();
                }
            }
            else this.bubbles.removeMovieClip();
            //
            var off=height-this._y;
            this.lava_step_i++;if(this.lava_step_i>lava_step){
                this.lava_step_i=0;
                this.lava_matrix.ty=random(off);
            }
            this.beginBitmapFill(volcano_bmp,this.lava_matrix,true);
            this.lineTo(lava_w,0);this.lineTo(lava_w,off);
            this.lineTo(0,off);this.lineTo(0,0);
            this.endFill();
        }
    }
    )");

    //mines

    int mine_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/4/mine.dbl",mine_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    action_sprite(presprite,R"(
        function hitTest_function(){
            removeMovieClip();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);
    swf_exports_add(sprite,"mine");
    //
    int one_secHit_alpha=80;int one_secHit_frames=fps;int one_secHit_alpha_decrement=one_secHit_alpha/one_secHit_frames;
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"_alpha=%u",one_secHit_alpha);
    swf_sprite_placeobject(presprite,hit_anim,0);swf_sprite_showframe(presprite);
    for(int i=0;i<one_secHit_frames;i++){
        actionf_sprite(presprite,buf,"_alpha-=%u",one_secHit_alpha_decrement);
        swf_sprite_showframe(presprite);
    }
    action_sprite(presprite,"removeMovieClip()");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);
    swf_exports_add(sprite,"generic_hit");

    actionf(buf,"var mine_w=%u;var mine_h=%u;var hit_anim_w=%u;var hit_anim_h=%u",mine_sz[0],mine_sz[1],hit_anim_sz[0],hit_anim_sz[1]);
    action(R"(
    function mines_init(){
        var z=zone.targets;
        zone_x_max=zone_x;var i=0;while(i<10){
            zone_x_max+=5+random(10*mine_w);
            var d=z.getNextHighestDepth();
            var mine=z.attachMovie('mine','mine'+d,d);
            mine._x=zone_x_max;mine._y=land_y-mine_h;
            mine.envTest_function=mines_env_test;
            zone_x_max+=mine_w;
            i++;
        }
    }
    var hit_anim_x_off=(hit_anim_w-mine_w)/2;var hit_anim_y_off=hit_anim_h-mine_h;
    //intoarce fals ca sa mai continue masina drumul
    function mines_env_test(){
        var x=this._x-hit_anim_x_off;var y=this._y-hit_anim_y_off;
        this.removeMovieClip();
        var impediments=zone.impediments;var d=impediments.getNextHighestDepth();
        var mine_blast=impediments.attachMovie('generic_hit','mine'+d,d);
        mine_blast._x=x;mine_blast._y=y;
        return false;
    }
    )");

    dbl=swf_dbl("C:/Desktop/universe/img/4/background.dbl");
    swf_exports_add(dbl,"background");
    action("var background_bmp=BitmapData.loadBitmap('background')");

    //traps

    int dust_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/4/dust.dbl",dust_sz);
    actionf_sprite(presprite,buf,"_alpha=%u",one_secHit_alpha);swf_sprite_placeobject(presprite,bmp,0);swf_sprite_showframe(presprite);
    for(int i=0;i<one_secHit_frames;i++){actionf_sprite(presprite,buf,"_alpha-=%u",one_secHit_alpha_decrement);swf_sprite_showframe(presprite);}
    action_sprite(presprite,"removeMovieClip()");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"dust");

    dbl=swf_dbl("C:/Desktop/universe/img/4/explosive.dbl");
    swf_exports_add(dbl,"explosive");
    action("var explosive_bmp=BitmapData.loadBitmap('explosive')");

    actionf(buf,"var dust_w=%u;var dust_h=%u",dust_sz[0],dust_sz[1]);
    action(R"(
        var traps_nr=2;var x_pos;var y_pos;
        var trap_step=24/fps;var gravity_y_add=trap_step/6;
        var trap_w=(traps_nr*2+1)*nature_object_w;var trap_h=(traps_nr*2+1)*nature_object_h;
        var y_pos_min=nature_object_h*traps_nr;
        var traps_array=new Array();
        //ca sa se puna background dinainte cand e
        var traps_total=5;
        function traps_pre_init(){
            traps_array.length=0;
            next_zone_x_max=zone_x;
            var n=0;while(n<traps_total){
                traps_array.push(next_zone_x_max);
                next_zone_x_max+=2*trap_w+random(10);
                n++;
            }
        }
        function traps_init(not_at_ground){
            zone_x_max=next_zone_x_max;
            var n=0;while(n<traps_total){
                var x_index=traps_array[n];
                var obstacles=zone.impediments;var d=obstacles.getNextHighestDepth();
                var activator=obstacles.createEmptyMovieClip('activator'+d,d);
                activator._x=x_index-width+car_mc._x+car_w+trap_w;
                activator._y=land_y;
                activator.lineStyle(1,0,0);activator.lineTo(dust_w,0);
                activator.envTest_function=activate_trap;
                activator.trap_rocks=new Array();
                //
                x_pos=x_index+(trap_w/2)-(nature_object_w/2);
                if(not_at_ground!=true)y_pos=land_y-nature_object_h;
                else y_pos=y_pos_min+random(height/2-y_pos_min);
                //
                var z=zone.aux;
                var d=z.getNextHighestDepth();
                var explosive_background=z.createEmptyMovieClip('explosive_background'+d,d);
                explosive_background._x=x_index;explosive_background._y=y_pos-(traps_nr*nature_object_h);
                explosive_background.beginBitmapFill(background_bmp,matrix,true);
                explosive_background.lineTo(trap_w,0);explosive_background.lineTo(trap_w,trap_h);
                explosive_background.lineTo(0,trap_h);explosive_background.lineTo(0,0);
                explosive_background.endFill();
                activator['explosive_background']=explosive_background;
                //
                var trg=zone.targets;var d=trg.getNextHighestDepth();
                var explosive=trg.createEmptyMovieClip('explosive'+d,d);
                explosive._x=x_pos;explosive._y=y_pos;
                explosive.beginBitmapFill(explosive_bmp,matrix,false);
                explosive.lineTo(nature_object_w,0);explosive.lineTo(nature_object_w,nature_object_h);
                explosive.lineTo(0,nature_object_h);explosive.lineTo(0,0);
                explosive.endFill();
                explosive.hitTest_function=explosive_hitTest;
                explosive['activator']=activator;
                activator['explosive']=explosive;
                //
                var x=x_pos;var c=0;while(c<traps_nr){x-=nature_object_w;
                    activator.trap_rocks.push(rock_trap(x,y_pos));c++;}
                var x=x_pos;var c=0;while(c<traps_nr){x+=nature_object_w;
                    activator.trap_rocks.push(rock_trap(x,y_pos));c++;}
                var y=y_pos;
                var y_nr=2*traps_nr+1;
                var x_start=x_pos-(traps_nr*nature_object_w);
                var c=0;while(c<traps_nr){
                    y-=nature_object_h;
                    x=x_start;
                    var i=0;while(i<y_nr){
                        activator.trap_rocks.push(rock_trap(x,y));
                        x+=nature_object_w;
                        i++;
                    }
                    c++;
                }
                //
                if(not_at_ground==true){
                    var y=y_pos;
                    var c=0;while(c<traps_nr){
                        y+=nature_object_h;
                        x=x_start;
                        var i=0;while(i<y_nr){
                            activator.trap_rocks.push(rock_trap(x,y));
                            x+=nature_object_w;
                            i++;
                        }
                        c++;
                    }
                }
                n++;
            }
        }
        //trap
        function rock_trap(x,y){
            var z=zone.targets;
            var d=z.getNextHighestDepth();
            var trap=z.createEmptyMovieClip('trap'+d,d);
            trap._x=x;trap._y=y;
            trap['keep_for_ground_contact']=true;
            var x_value=(x-x_pos)/nature_object_w;
            var rotation_step=x_value*rot_step;
            com.mc_rotation_bmp_show(flash.display.BitmapData.loadBitmap(nature_objects_types[random(nature_objects_types.length)]+'_bmp'),trap,'img',0,rotation_step);
            //
            trap['x_step']=x_value*trap_step*4;
            trap['y_step']=(y-y_pos)/nature_object_h*trap_step*2;
            trap['nature_object_move']=nature_object_move;
            trap.hitTest_function=function(){this.removeMovieClip();}
            return trap;
        }
        //false
        function activate_trap(){
            for(var elem in this.trap_rocks){
                var mc=this.trap_rocks[elem];
                //se poate sa fie nimerit cu bulletsurile si sa fie undefined
                if(mc!=undefined){
                    mc.onEnterFrame=function(){
                        this.y_step+=gravity_y_add;
                        this.nature_object_move();
                        this.img.rotate_bmp();
                    }
                }
            }
            var d=zone.getNextHighestDepth();
            var dust=zone.attachMovie('dust','dust'+d,d);
            dust._x=this._x;dust._y=this._y-dust_h;
            //
            var explosive_background=this.explosive_background;
            var x=explosive_background._x;var y=explosive_background._y;var w=explosive_background._width;var h=explosive_background._height;
            explosive_background.removeMovieClip();
            this.explosive.removeMovieClip();
            var z=zone.aux;
            var d=z.getNextHighestDepth();
            var hit=z.attachMovie('generic_hit','generic_hit'+d,d);
            hit._x=x;hit._y=y;
            hit._xscale=w/hit_anim_w*100;hit._yscale=h/hit_anim_h*100;
            //
            this.removeMovieClip();
            //
            return false;
        }
        function explosive_hitTest(){
            this.activator.envTest_function();
        }

    )");

    //

    action(R"(
        var cavern_entrance_parts=6;
        var cEntrance_y_ratio=land_y/cavern_entrance_parts;var cEntrance_x_ratio=width/cavern_entrance_parts;
        function cavern(x){
            var d=background.getNextHighestDepth();
            var bgr=background.createEmptyMovieClip('bgr'+d,d);bgr._x=x;
            bgr.beginBitmapFill(background_bmp,matrix,true);
            bgr.moveTo(0,land_y);
            cavern_entrance(bgr,0-cEntrance_y_ratio,0,land_y);
            bgr.lineTo(next_zone_x_max,0);
            cavern_entrance(bgr,cEntrance_y_ratio,next_zone_x_max,0);
            bgr.lineTo(0,land_y);
            bgr.endFill();
        }
        function next_cavern(ramp_is_double){
            next_x_max_stoper(ramp_is_double);
            cavern(zone_x_max);
        }
        function cavern_entrance(bgr,y_ratio,x,y){
            var i=0;while(i<cavern_entrance_parts){
                if((i&1)==0)bgr.curveTo(x,y+y_ratio,x+cEntrance_x_ratio,y+y_ratio);
                else bgr.curveTo(x+cEntrance_x_ratio,y,x+cEntrance_x_ratio,y+y_ratio);
                y+=y_ratio;x+=cEntrance_x_ratio;
                i++;
            }
        }
    )");

    //

    dbl=swf_dbl("C:/Desktop/universe/img/4/sky.dbl");
    swf_exports_add(dbl,"sky");
    action(R"(
        var sky_bmp=BitmapData.loadBitmap('sky');
        createEmptyMovieClip('sky',getNextHighestDepth());
        sky.beginBitmapFill(sky_bmp,matrix,true);
        sky.lineTo(width*2,0);sky.lineTo(width*2,land_y);
        sky.lineTo(0,land_y);sky.lineTo(0,0);
        sky.endFill();
        //
        createEmptyMovieClip('background',getNextHighestDepth());
    )");

    actionf(buf,"var ground_w=%u",ground_sz[0]);

    action(R"(
    var gun;var wheelA;var wheelB;var car;
    attachMovie('car_mc','car_mc',getNextHighestDepth());
    car_mc._x=100;car_mc._y=land_y-car_h;
    car_mc.onEnterFrame=car_control;

    //
    var subzones=new Array('aux','ground','impediments','ramps','targets');
    var zone_nr=0;
    //
    var zone_x=width;var zone_x_max;
    var ground_bmp=BitmapData.loadBitmap('ground');
    var inter_ground_rest=0;
    //
    function inter_ground_set(){
        var ground_sum=zone._x*-1+inter_ground_rest;
        inter_ground_rest=ground_sum-(int(ground_sum/ground_w)*ground_w);
        //
        for(var z in background){
            //zone._x e negativ
            var bgr=background[z];bgr._x+=zone._x;
            if(bgr._x+bgr._width<0)bgr.removeMovieClip();
        }
        //
        zone.ground.clear();
        for(var z in zone.impediments)zone.impediments[z].removeMovieClip();
        for(var z in zone.ramps)zone.ramps[z].removeMovieClip();
        for(var z in zone.targets){
            var target=zone.targets[z];
            if(target.keep_for_ground_contact!=true)target.removeMovieClip();
            //zone._x e negativ
            else target._x+=zone._x;
        }
        //trag o linie ca sa mai faca si pietrele care raman contactul
        //nu, mai bine am pus versus land_y ca la prima nu are si poate fac niste trape care vor cadea pe langa
        //
        zone._x=0;background._x=0;
        //asta trebuie doar la cele cu onEnterFrame
        zone.onEnterFrame=undefined;
    }

    d=getNextHighestDepth();
    createEmptyMovieClip('zone',d);
    for(var z in subzones)zone.createEmptyMovieClip(subzones[z],z);
    zone.ground._y=land_y;
    //
    createEmptyMovieClip('bullets',getNextHighestDepth());
    //
    function zone_proc(nr,not_a_background_request){
        if(nr==0){
            if(not_a_background_request==true)add_bridge();
            else{bridge_init();cavern(0);}
        }
        else if(nr==1){
            if(not_a_background_request==true)add_bridge(true);
            else{bridge_init(true);cavern(zone_x_max);}
        }
        else if(nr==2){
            if(not_a_background_request==true)fall_init(nature_objects_types[0]);
        }
        else if(nr==3){
            if(not_a_background_request==true)add_volcano();
            else next_cavern();
        }
        else if(nr==4){
            if(not_a_background_request==true)add_volcano(true);
            else next_cavern(true);
        }
        else if(nr==5){
            if(not_a_background_request==true)fall_init(nature_objects_types[1]);
        }
        else if(nr==6){
            if(not_a_background_request==true)mines_init();
        }
        else if(nr==7){
            if(not_a_background_request==true)fall_init();
        }
        else if(nr==8){
            if(not_a_background_request==true)add_water();
            else next_cavern();
        }
        else if(nr==9){
            if(not_a_background_request==true)add_water(true);
            else next_cavern(true);
        }
        else if(nr==10){
            if(not_a_background_request==true)traps_init();
            //la x_index ca sa faca potrivirea
            else traps_pre_init();
        }
        else if(nr==11){
            if(not_a_background_request==true)add_lava();
            else next_cavern();
        }
        else if(nr==12){
            if(not_a_background_request==true)add_lava(true);
            else next_cavern(true);
        }
        else if(nr==13){
            if(not_a_background_request==true)traps_init(true);
            else{traps_pre_init();cavern(zone_x_max);}
        }
        //14
        else if(not_a_background_request==true)_root.end_scenario();
    }
    var next_zone_x_max;
    //
    function new_zone(){
        var gr=zone.ground;
        //
        zone_proc(zone_nr,true);
        zone_proc(zone_nr+1,false);
        //asta e doar pentru land sa se vada la sfarsit ca masina nu mai merge mai departe de zone_x_max-width
        zone_x_max+=width;

        //
        var ground_x_max=inter_ground_rest+zone_x_max;
        gr.beginBitmapFill(ground_bmp,matrix,true);
        gr.lineTo(ground_x_max,0);gr.lineTo(ground_x_max,land_to_height);gr.lineTo(0,land_to_height);gr.lineTo(0,0);
        gr.endFill();
        gr._x=0-inter_ground_rest;
        //
        zone_nr++;
    }
    )");

    swf_exports_done();
    swf_showframe();
    swf_done();
	printf("\ndone");
    return 1;
}


#ifdef __cplusplus
}
#endif
