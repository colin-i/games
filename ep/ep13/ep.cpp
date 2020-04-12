

#include <Windows.h>
#include<stdio.h>
#include <Math.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

int _start(){
    char* ep_nm=Rooms_ep;
	sprintf(buf, "%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);
    action("_root['episode']=this");
        //set_init(Rooms_keys);
        init_episod(ep_nm);

    #define step height/(5*fps)
    #define targets_step step/2
    #define bullet_step height/(2*fps)

    #define margin_inset 10

    //player

    int ship_body_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/13/ship.dbl",ship_body_sz);
    swf_exports_add(dbl,"ship_body");
	//
	automatic(3,"if(auto_move==false)player.clear_back();");
	//
    int frontala=ship_body_sz[0]/2;
    int side_break=frontala*40/100;
    int front_break=side_break/2;
    int inset_sz=frontala*20/100;
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var step=%u;var rot_step=%u",step,180/fps);
    actionf_sprite(presprite,buf,"var lat_sz=%u;var inset_sz=%u;var side_break=%u;var front_break=%u",ship_body_sz[0],inset_sz,side_break,front_break);
    actionf_sprite(presprite,buf,"var margin_inset=%u",margin_inset);
	actionf_sprite(presprite, buf, "var shot_interval=%u", shot_interval);
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        //
        var half=lat_sz/2;
        var front_half=lat_sz/4;
        var half_side_break=side_break/2;
        var half_front_break=front_break/2;
        //
        var mc=createEmptyMovieClip('body',getNextHighestDepth());
        //
        var ship_bmp=flash.display.BitmapData.loadBitmap('ship_body');
        var mx=new flash.geom.Matrix();
        mx.tx=0-half;mx.ty=0-half;
        mc.beginBitmapFill(ship_bmp,mx,false);
        //
        mc.moveTo(mx.tx,mx.ty);
        //
        var sideFuel_x=0-half_side_break;var sideFuelLeft_y=0-half+inset_sz;
        mc.lineTo(sideFuel_x,sideFuelLeft_y);
        mc.lineTo(half_side_break,0-half+inset_sz);
        var frontFuelLeft_y=0-front_half-half_front_break;
        var frontFuel_x=half-inset_sz;
        mc.curveTo(frontFuel_x,0-half+inset_sz,frontFuel_x,frontFuelLeft_y);
        mc.lineTo(frontFuel_x,0-front_half+half_front_break);
        mc.lineTo(half,0);
        //
        var frontFuelRight_y=front_half-half_front_break;
        mc.lineTo(half-inset_sz,frontFuelRight_y);
        mc.lineTo(frontFuel_x,front_half+half_front_break);
        mc.curveTo(frontFuel_x,half-inset_sz,half_side_break,half-inset_sz);
        var sideFuelRight_y=half-inset_sz;
        mc.lineTo(0-half_side_break,sideFuelRight_y);
        mc.lineTo(0-half,half);
        //
        mc.lineTo(0-half,0-half);
        mc.endFill();
        //
        com.add_fuel(side_break,side_break,1,'backFuel',this);backFuel._x=0-half;backFuel._y=0-half_side_break;backFuel._rotation=90;
        //in fata au aceeasi inaltime
        //ambele -1 ca sa fie sincronizate
        com.add_fuel(front_break,side_break,-1,'frontFuelLeft',this);frontFuelLeft._x=frontFuel_x;frontFuelLeft._y=frontFuelLeft_y;frontFuelLeft._rotation=90;
        com.add_fuel(front_break,side_break,-1,'frontFuelRight',this);frontFuelRight._x=frontFuel_x;frontFuelRight._y=frontFuelRight_y;frontFuelRight._rotation=90;
        //
        com.add_fuel(side_break,side_break,-1,'sideFuelLeft',this);sideFuelLeft._x=sideFuel_x;sideFuelLeft._y=sideFuelLeft_y;
        com.add_fuel(side_break,side_break,1,'sideFuelRight',this);sideFuelRight._x=sideFuel_x;sideFuelRight._y=sideFuelRight_y;
        //
		function backward(){
			move_decision(0-Math.cos(_rotation*Math.PI/180),0-Math.sin(_rotation*Math.PI/180));
            frontFuelLeft.fuelOn();
            frontFuelRight.fuelOn();
		}
        onEnterFrame=function(){
            if(Key.isDown(com.left_arrow)==true){
                _rotation-=rot_step;if(_rotation<0)_rotation+=360;
            }
            else if(Key.isDown(com.right_arrow)==true){
                _rotation+=rot_step;if(_rotation>360)_rotation-=360;
            }
            if(Key.isDown(com.up_arrow)==true){
                move_decision(Math.cos(_rotation*Math.PI/180),Math.sin(_rotation*Math.PI/180));
//in caz ca e apasat-spate/automat-spate
				clear_back();
                backFuel.fuelOn();
            }
            else if(Key.isDown(com.down_arrow)==true)backward();
			else if(ep.auto_move==true)backward();
            if(Key.isDown(com.strafe_left)==true){
                move_decision(Math.cos((_rotation-90)*Math.PI/180),Math.sin((_rotation-90)*Math.PI/180));
                sideFuelRight.fuelOn();
            }
            else if(Key.isDown(com.strafe_right)==true){
                move_decision(Math.cos((_rotation+90)*Math.PI/180),Math.sin((_rotation+90)*Math.PI/180));
                sideFuelLeft.fuelOn();
            }
            if(Key.isDown(com.action_key)==true)shot_func();
			else if(ep.auto_shot==true)shot_func();
			if(frames_shot>0)frames_shot--;
        }
		var frames_shot=0;
		function shot_func(){
			if(frames_shot==0){
				frames_shot=shot_interval;
				var d=ep.targets_in.getNextHighestDepth();
		        var mc=ep.targets_in.attachMovie('bullet','bullet'+d,d);
			    mc['starter']=this;
			}
		}
        function move_decision(x_factor,y_factor){
            var test_x=_x+(x_factor*step);
            if(test_x<(0+margin_inset))_x=0+margin_inset;
            else if(width-margin_inset<test_x)_x=width-margin_inset;
            else _x=test_x;
            var test_y=_y+(y_factor*step);
            if(test_y<(0+margin_inset))_y=0+margin_inset;
            else if(height-margin_inset<test_y)_y=height-margin_inset;
            else _y=test_y;
        }
		function clear_back(){
			frontFuelLeft.clear();
            frontFuelRight.clear();
		}
        Key.addListener(this);
        onKeyUp=function(){
            if(Key.getCode()==com.up_arrow){
                backFuel.clear();
            }
            else if(Key.getCode()==com.down_arrow)clear_back();
            else if(Key.getCode()==com.strafe_left){
                sideFuelRight.clear();
            }
            else if(Key.getCode()==com.strafe_right){
                sideFuelLeft.clear();
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"player");

    //margin
    dbl=swf_dbl("C:/Desktop/universe/img/12/margin.dbl");
    swf_exports_add(dbl,"margin_texture");
    action("var margin_bmp=flash.display.BitmapData.loadBitmap('margin_texture');var margin_mx=new flash.geom.Matrix()");
    //
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        //
        beginBitmapFill(ep.margin_bmp,ep.margin_mx,true);
        lineTo(wd,0);lineTo(wd,hg);
        lineTo(0,hg);lineTo(0,0);
        endFill();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"margin_mc");

    //
    char nm[100]; char nm2[100]; char spc[200];
    for(int a=0;a<3;a++){
        for(int i=0;i<4;i++){
            const char* type=(i==0?"normal":(i==1?"double":(i==2?"fast":"boss")));
            sprintf(nm,"set%u_%s",a,type);
            sprintf(spc,"C:/Desktop/universe/img/13/%s.dbl",nm);
            dbl=swf_dbl(spc);
			sprintf(nm2, "set%u_%u", a, i);
            swf_exports_add(dbl, nm2);
            sprintf(spc,"var %s=flash.display.BitmapData.loadBitmap('%s')", nm2, nm2);
            action(spc);
        }
    }

    //targets_in
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var max_hitPoints=%u",fps/2);
    actionf_sprite(presprite,buf,"var step=%u",targets_step);
	actionf_sprite(presprite, buf, "var shot_interval=%u", shot_interval);
    action_sprite(presprite,"var mc_at_start=4;var mc_reserve=3");
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        var mx=new flash.geom.Matrix();
        //
        var bmp_name='set'+set+'_'+type;
//==0?'normal':(type==1?'double':(type==2?'fast':'boss')));
        var zone_bmp=ep[bmp_name];
        var wd=zone_bmp.width;var hg=zone_bmp.height;
        mx.tx=0-(wd/2);mx.ty=0-(hg/2);
        //
        var hitPoints_h=5;var y_point=mx.ty-5;
        function drawHealth(mv){
            var mc=mv.hitpointsBar;
            mc.clear();
            mc.beginFill(0x00ff00);
            var point_off=zone_bmp.width*mv.hitPoints/max_hitPoints;
            mc.moveTo(mx.tx,y_point);
            mc.lineTo(mx.tx+point_off,y_point);
            mc.lineTo(mx.tx+point_off,y_point-hitPoints_h);
            mc.lineTo(mx.tx,y_point-hitPoints_h);
            mc.endFill();
        }
        function add_target(){
            var d=ep.targets.getNextHighestDepth();
            var mc=ep.targets.createEmptyMovieClip(bmp_name+d,d);
            //
            var mv_main=mc.createEmptyMovieClip('main',mc.getNextHighestDepth());
            var mv=mv_main.createEmptyMovieClip('body',mv_main.getNextHighestDepth());
            mv.beginBitmapFill(zone_bmp,mx,false);
            mv.moveTo(mx.tx,mx.ty);
            mv.lineTo(wd/2,mx.ty);
            mv.lineTo(wd/2,hg/2);
            mv.lineTo(mx.tx,hg/2);
            mv.endFill();
            //
            mc.createEmptyMovieClip('hitpointsBar',mc.getNextHighestDepth());
            //
            var nr1=random(2);var nr2=nr1^1;
            mc._x=random(2)*width*nr1+(random(width)*nr2);
            mc._y=random(height)*nr1+(random(2)*height*nr2);
            mc['step']=step;
            mc.onEnterFrame=function(){
                if(this.main.body.hitTest(ep.player.body)==false){
                    var rect=ep.player.getBounds(this);
                    var x_center=(rect.xMax-rect.xMin)/2+rect.xMin;
                    var y_center=(rect.yMax-rect.yMin)/2+rect.yMin;
                    var rot=Math.atan2(y_center,x_center)*180/Math.PI;
                    this.main._rotation=rot;
                    this._x+=Math.cos(rot*Math.PI/180)*this.step;this._y+=Math.sin(rot*Math.PI/180)*this.step;
                    this.main.backFuel.fuelOn();
                }
                else{
                    this.main.backFuel.clear();
                    if(this.hitPoints<max_hitPoints){
                        this.hitPoints++;drawHealth(this);
                    }
                }
            }
            com.add_fuel(hg/2,wd/2,1,'backFuel',mv_main);mv_main.backFuel._x=mx.tx;mv_main.backFuel._y=mx.ty/2;mv_main.backFuel._rotation=90;
            mc['hitPoints']=max_hitPoints;
            drawHealth(mc);
            mc.hitTest_function=function(){
                this.hitPoints-=shot_interval;
                if(this.hitPoints>0)drawHealth(this);
                else{
                    if(mc_reserve==0){
			var nr=0;
                        for(var mc_name in ep.targets)nr++;
                        //a ramas doar asta
                        if(nr==1)ep.open_door();
		    }
                    else{
			mc_reserve--;
			add_target();
		    }
                    this.removeMovieClip();
                }
            }
        }
        //
        var index=0;
	if(set>0)max_hitPoints*=set*5;
        if(type&1!=0)max_hitPoints*=2;
        if(type&2!=0)step*=2;
	if(type==3){max_hitPoints*=mc_at_start+mc_reserve;index=mc_at_start-1;mc_reserve=0;}
        for(;index<mc_at_start;index++)add_target();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"targets_in");

    //bullet
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var ship_lat=%u",ship_body_sz[0]);
    actionf_sprite(presprite,buf,"var step=%u",bullet_step);
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    actionf_sprite(presprite,buf,"var fade_out_total=%u",fps);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        //
        _x=starter._x;_y=starter._y;
        var x_factor=Math.cos(starter._rotation*Math.PI/180);
        var y_factor=Math.sin(starter._rotation*Math.PI/180);
        _x+=x_factor*ship_lat;_y+=y_factor*ship_lat;
        var x_step=x_factor*step;var y_step=y_factor*step;
        //
        var bullet_r=3;
        beginFill(0xff8000);
        com.drawCircle_offset(this,bullet_r,0-bullet_r);
        endFill();
        //
        onEnterFrame=function(){
            if(bullet_move()==false){
                var frames=fade_out_total;
                onEnterFrame=function(){
                    frames--;
                    if(frames==0)removeMovieClip();
                    else _alpha=100*frames/fade_out_total;
                }
            }
            else{
                for(var mc_name in ep.targets){
                    var mc=ep.targets[mc_name];
                    if(hitTest(mc.main.body)==true){
                        mc.hitTest_function();
                        removeMovieClip();
                        break;
                    }
                }
            }
        }
        //bool
        function bullet_move(){
            var test_x=_x+x_step;
            if(test_x<0)return false;
            if(width<test_x)return false;
            var test_y=_y+y_step;
            if(test_y<0)return false;
            if(height<test_y)return false;
            _x=test_x;_y=test_y;
            return true;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bullet");

    actionf(buf,"var width=%u;var height=%u",width,height);
    actionf(buf,"var fps=%u",fps);
    actionf(buf,"var hole_spc=%u",ship_body_sz[0]/3);
    actionf(buf,"var margin_inset=%u",margin_inset);
	action(R"(
        createEmptyMovieClip('bg',getNextHighestDepth());
        //
        var mc;var d;
        //
        d=getNextHighestDepth();mc=attachMovie('margin_mc','margin_mc'+d,d);
        mc['wd']=width;mc['hg']=margin_inset;
        //
        d=getNextHighestDepth();mc=attachMovie('margin_mc','margin_mc'+d,d);
        mc['wd']=margin_inset;mc['hg']=height;
        //
        createEmptyMovieClip('door',getNextHighestDepth());
        mc=door.attachMovie('margin_mc','left_door',door.getNextHighestDepth());
        mc['wd']=width/2-(hole_spc/2);mc['hg']=0-margin_inset;
        mc._y=height;
            //
        mc=door.attachMovie('margin_mc','right_door',door.getNextHighestDepth());
        mc['wd']=0-door.left_door.wd;mc['hg']=0-margin_inset;
        mc._y=height;
        //
        d=getNextHighestDepth();mc=attachMovie('margin_mc','margin_mc'+d,d);
        mc['wd']=0-margin_inset;mc['hg']=height;
        mc._x=width;
        //
        attachMovie('player','player',getNextHighestDepth());
        //
        createEmptyMovieClip('targets',getNextHighestDepth());

        var zoneNr=0;
        function new_zone(){
            bg.env.removeMovieClip();
            com.space_env(bg,'env',bg.getNextHighestDepth());
            //
            door.left_door._x=0;door.right_door._x=width;
            player._x=width/2;player._y=height/2;
            //
            targets_in.removeMovieClip();
            //
            var setNr=int(zoneNr/4);var partNr=zoneNr%4;
            //
            attachMovie('targets_in','targets_in',getNextHighestDepth());
            //
            targets_in['set']=setNr;
            if(partNr==0)targets_in['type']=0;
            else if(partNr==1)targets_in['type']=1;
            else if(partNr==2)targets_in['type']=2;
            else targets_in['type']=3;
            //
            zoneNr++;
        }
        new_zone();

        var door_open_frames=2*fps;
        function open_door(){
			attachMovie('tip','tip',getNextHighestDepth());tip.width=width;tip.height=height;tip.fps=fps;
            var frames=0;
            door.onEnterFrame=function(){
                if(this.hitTest(player.body)==false){
                    if(frames<door_open_frames){
                        frames++;
                        var moveDist=(width/2)*frames/door_open_frames;
                        this.left_door._x=0-moveDist;this.right_door._x=width+moveDist;
                    }
                }
                else{
                    if(zoneNr<12){tip.removeMovieClip();this.onEnterFrame=undefined;new_zone();}
                    else _root.end_scenario();
                }
            }
        }
    )");
	presprite = swf_sprite_new();
	action_sprite(presprite, R"(
var wd=50;var hg=75;b=hg/3;c=wd/2;a=c/3;_x=(width-wd)/2;_y=(height-hg)/2;
var frames=0;var on=false;
onEnterFrame=function(){
	if(frames==0){
		if(on==false){
			on=true;
			beginFill(0x00ff00);
			moveTo(c-a,0);lineTo(c+a,0);lineTo(c+a,hg-b);lineTo(wd,hg-b);lineTo(c,hg);lineTo(0,hg-b);lineTo(c-a,hg-b);endFill();
		}else{clear();on=false;}
		frames=fps;
	}else frames--;
}
)");
	sprite = swf_sprite_done(presprite); swf_exports_add(sprite, "tip");

    swf_exports_done();
    swf_showframe();
    swf_done();
	printf("\ndone");
    return 1;
}

#ifdef __cplusplus
}
#endif
