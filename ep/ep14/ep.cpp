
#include <Windows.h>
#include<stdio.h>
#include <Math.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

int _start(){
    char* ep_nm=Road_ep;
	sprintf(buf, "%s.swf", ep_nm);
	swf_new(buf,width,height,0x01ff01,fps);
    action("_root['episode']=this");
        //set_init(Road_keys);
        init_episod(ep_nm);

    #define step height/(3*fps)

    //ship
    //ship_body
    int ship_body_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/12/ship.dbl",ship_body_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(ship_body_sz[0]/2),0-(ship_body_sz[1]/2));
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        ep.ship['bodyRect']=getBounds(this);
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_body");
    //
	automatic(1);
	//
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var ship_body_w=%u;var ship_body_h=%u",ship_body_sz[0],ship_body_sz[1]);
    actionf_sprite(presprite,buf,"var step=%u;var rot_step=%u",step,90/fps);
    actionf_sprite(presprite,buf,"var shot_frames=%u",shot_interval);
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        //
        com.addFuel(ship_body_w,ship_body_h/2,1,'fuel',this);
        fuel._y=ship_body_h/2;
        //
        var strafe_fuel_sz=ship_body_h/2;
        com.addFuel(ship_body_w/4,strafe_fuel_sz,1,'rightFuel',this);
        rightFuel._rotation=-90;rightFuel._x=ship_body_w/4;
        //
        com.addFuel(ship_body_w/4,strafe_fuel_sz,1,'leftFuel',this);
        leftFuel._rotation=90;leftFuel._x=0-(ship_body_w/4);
        //
        attachMovie('ship_body','body',getNextHighestDepth());
        var frame_step;
        onEnterFrame=function(){
            ep.rot_0();
            frame_step=step;
            var on_the_road=false;
            var mv=ep.env_container.objects.roads;
            for(var mc_name in mv){
                var mc=mv[mc_name];
                if(body.hitTest(mc)==true){on_the_road=true;break;}
            }
            if(on_the_road==false)frame_step/=10;
            //
            var slowers=ep.env_container.objects.slowers;
            for(var mc_name in slowers){
                if(body.hitTest(slowers[mc_name])==true){
                    frame_step/=2;
                    break;
                }
            }
            //
            var targets=ep.env_container.objects.targets;
            for(var mc_name in targets){
                if(body.hitTest(targets[mc_name])==true){
                    frame_step/=2;
                    break;
                }
            }
            //
            var fasters=ep.env_container.objects.fasters;
            for(var mc_name in fasters){
                if(body.hitTest(fasters[mc_name])==true){
                    frame_step*=2;
                    break;
                }
            }
            //
            ep.rot_back();
            if(splash!=undefined)frame_step/=2;
            //
            fuel.fuelOn();
            if(Key.isDown(com.left_arrow)==true){
                ep.env_container._rotation+=rot_step;
                //if(ep.env_container._rotation<0)ep.env_container._rotation+=360;
            }
            else if(Key.isDown(com.right_arrow)==true){
                ep.env_container._rotation-=rot_step;
                //if(ep.env_container._rotation>360)ep.env_container._rotation-=360;
            }
            if(Key.isDown(com.strafe_left)==true){
                rightFuel.fuelOn();
                move_env(90);
            }
            else if(Key.isDown(com.strafe_right)==true){
                leftFuel.fuelOn();
                move_env(-90);
            }
            if(Key.isDown(com.action_key)==true)shot_func();
			else if(ep.auto_shot==true)shot_func();
            move_env(0);
            //
            ep.rot_0();
            var solids=ep.env_container.objects.solids;
            for(var mc_name in solids){
                if(body.hitTest(solids[mc_name])==true){
                    hit_test(solids[mc_name]);
                    break;
                }
            }
            ep.rot_back();
        }
		function shot_func(){
			if(bullet==undefined){
				createEmptyMovieClip('bullet',getNextHighestDepth());
                var frames=shot_frames;
                bullet.onEnterFrame=function(){
					frames--;if(frames>0)return null;
                    this.removeMovieClip();
                }
                //
                var d=ep.env_container.objects.bullets.getNextHighestDepth();
                ep.env_container.objects.bullets.attachMovie('bullet','bullet'+d,d);
            }
		}
        Key.addListener(this);
        onKeyUp=function(){
            if(Key.getCode()==com.strafe_left){
                rightFuel.clear();
            }
            else if(Key.getCode()==com.strafe_right){
                leftFuel.clear();
            }
        }
        function move_env(rot){
            rot+=ep.env_container._rotation;
            ep.env_container.objects._x+=Math.sin(rot*Math.PI/180)*frame_step;
            ep.env_container.objects._y+=Math.cos(rot*Math.PI/180)*frame_step;
        }
        function hit_test(fixed_mc){
            var fxRect=fixed_mc.getBounds(body);
            //
            var d_moveLeft=fxRect.xMin-bodyRect.xMax;
            var d_moveUp=fxRect.yMin-bodyRect.yMax;
            var d_moveRight=fxRect.xMax-bodyRect.xMin;
            var d_moveDown=fxRect.yMax-bodyRect.yMin;
            //
            var d_min=Math.min(Math.abs(d_moveLeft),Math.abs(d_moveUp));
            d_min=Math.min(d_min,Math.abs(d_moveRight));
            d_min=Math.min(d_min,Math.abs(d_moveDown));
            //
            if(d_min==Math.abs(d_moveLeft))ep.env_container.objects._x+=d_moveLeft*-1;
            else if(d_min==Math.abs(d_moveUp))ep.env_container.objects._y+=d_moveUp*-1;
            else if(d_min==Math.abs(d_moveRight))ep.env_container.objects._x+=d_moveRight*-1;
            else ep.env_container.objects._y+=d_moveDown*-1;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship");
    //splash
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var area=%u",2*ship_body_sz[0]);
    action_sprite(presprite,R"(
        var com=_root['shared_level'];
        var mx=new flash.geom.Matrix();
        var colors=new Array(0xff0000,0xffFF00);
        //
        var start_alpha=70;
        var alphas=new Array(start_alpha,start_alpha);
        var ratios=new Array(0,255*area/Stage.width);
        onEnterFrame=function(){
            frames--;
            if(frames>0){
                alphas[0]=start_alpha*frames/total_frames;alphas[1]=alphas[0];
                clear();
                beginGradientFill('radial',colors,alphas,ratios,mx);
                com.drawCircle_offset(this,area,0-area);
                endFill();
            }
            else removeMovieClip();
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"splash");

    //target
    int target_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/10/target.dbl",target_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"red_target");

    //bullet
        //ca sa nu rateze tinta
    int bullet_step=target_sz[0];
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var half_ship=%u",ship_body_sz[0]/2);
    actionf_sprite(presprite,buf,"var step=%u;var total_frames=%u",bullet_step,(2*height)/bullet_step);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        //
        var bullet_r=3;
        beginFill(0xff8000);
        com.drawCircle_offset(this,bullet_r,0-bullet_r);
        endFill();
        //
        _x=0-ep.env_container.objects._x;_y=0-ep.env_container.objects._y;
        //
        var angle=ep.env_container._rotation-90;
        var x_step=0-(step*Math.cos(angle*Math.PI/180));
        var y_step=step*Math.sin(angle*Math.PI/180);
        var frames=total_frames;
        onEnterFrame=function(){
            frames--;if(frames<0)removeMovieClip();
            else{
                _x+=x_step;_y+=y_step;
                var targets=ep.env_container.objects.targets;
                var hit=false;
                ep.rot_0();
                for(var mc_name in targets){
                    if(hitTest(targets[mc_name])==true){
                        hit=true;
                        targets[mc_name].target.hitTest_function();
                        targets[mc_name].removeMovieClip();
                        break;
                    }
                }
                ep.rot_back();
                if(hit==true)removeMovieClip();
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bullet");

    int latura_mica_jumate=2*ship_body_sz[0];
    int latura_mare=2*height;

    //road
	//asta ca sa fie linia neintrerupta la sfarsit, ca fapt divers, nu poate fi 20 ca index_n va fi mai mare/cate-s=20+
	actionf(buf, R"(
		var compute_index=0;compute_n=0;while(compute_n<%u){compute_n+=20;compute_index++;}
		var compute_one=%u/compute_index;
	)", latura_mare - latura_mica_jumate, latura_mare - latura_mica_jumate);
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var small_lat_half=%u;var long_lat=%u",latura_mica_jumate,latura_mare);
	action_sprite(presprite, R"(
        beginFill(0xffFFff,40);
        lineTo(0,small_lat_half);lineTo(long_lat,small_lat_half);
        lineTo(long_lat,0-small_lat_half);lineTo(0,0-small_lat_half);
        endFill();
		lineStyle(1,0xffFFff,50);
		if(end_line==true){
			moveTo(long_lat-1,0-small_lat_half+1);
			lineTo(long_lat-1,small_lat_half-1);
		}
		var ep=_root['episode'];
		var x=small_lat_half;var full=ep.compute_one;var half=full/2;
		if(inter!=undefined){
			var y=(small_lat_half-1)*inter;
			y-=full*inter;while(y*inter>0){
				moveTo(x,y);lineTo(x,y+(half*inter));y-=full*inter;
			}
		}
		var i=0;while(i<ep.compute_index){i++;x+=full;moveTo(x,0);lineTo(x-half,0);}
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"road");

    int items_roadStart=(2*latura_mica_jumate)+ship_body_sz[0];
    int items_defaultUp=0-latura_mica_jumate;
    int slows_defaultBottom=0;

    //obstacles
    //jaloane solide miscatoare
    int obst_small_lat=ship_body_sz[0];
    int inter_obst=5*obst_small_lat;
    int obst_Bottom=latura_mica_jumate/2;
    dbl=swf_dbl("C:/Desktop/universe/img/14/jaloane.dbl");
    swf_exports_add(dbl,"obst_txt");
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var objects_roadStart=%u;var long_lat=%u",items_roadStart,latura_mare);
    actionf_sprite(presprite,buf,"var obst_small_lat=%u;var inter_obst=%u",obst_small_lat,inter_obst);
    actionf_sprite(presprite,buf,"var up_y=%u;var bottom_y=%u",items_defaultUp,obst_Bottom);
    //
    int movingObst_space=2*ship_body_sz[0];
    actionf_sprite(presprite,buf,"var movingObst_space=%u",movingObst_space);
    actionf_sprite(presprite,buf,"var small_half_lat=%u",latura_mica_jumate);
    actionf_sprite(presprite,buf,"var step=%u",movingObst_space/fps);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var solids=ep.env_container.objects.solids;
        var obst_bmp=flash.display.BitmapData.loadBitmap('obst_txt');
        var mx=new flash.geom.Matrix();
        function add_obst(road){
            var pos_pointer=objects_roadStart;
            if(flag!=true){
                var factor=random(2)*2-1;
                while(pos_pointer<long_lat){
                    var d=solids.getNextHighestDepth();var mc=solids.createEmptyMovieClip('obstacle'+d,d);
                    mc._x=road._x;mc._y=road._y;mc._rotation=road._rotation;
                    mc.beginBitmapFill(obst_bmp,mx,true);
                    mc.moveTo(pos_pointer,up_y*factor);
                    mc.lineTo(pos_pointer+obst_small_lat,up_y*factor);
                    mc.lineTo(pos_pointer+obst_small_lat,bottom_y*factor);
                    mc.lineTo(pos_pointer,bottom_y*factor);
                    mc.endFill();
                    factor*=-1;pos_pointer+=obst_small_lat+inter_obst;
                }
            }else{
                while(pos_pointer<long_lat){
                    var movingObst_dist=2*obst_small_lat;
                    var all_dist=2*small_half_lat-movingObst_space;
                    var d;
                    //
                    d=solids.getNextHighestDepth();var mc=solids.createEmptyMovieClip('obstacle'+d,d);
                    mc._x=road._x;mc._y=road._y;mc._rotation=road._rotation;
                    mc=mc.createEmptyMovieClip('obstacle',mc.getNextHighestDepth());
                    mc.beginBitmapFill(obst_bmp,mx,true);
                    mc._x=pos_pointer;mc._y=up_y;
                    mc['left_lim']=mc._y-all_dist;mc['right_lim']=mc.left_lim+all_dist;
                    mc.lineTo(movingObst_dist,0);
                    mc.lineTo(movingObst_dist,all_dist);
                    mc.lineTo(0,all_dist);
                    mc.endFill();
                    //
                    d=solids.getNextHighestDepth();var mv=solids.createEmptyMovieClip('obstacle'+d,d);
                    mv._x=road._x;mv._y=road._y;mv._rotation=road._rotation;
                    mv=mv.createEmptyMovieClip('obstacle',mv.getNextHighestDepth());
                    mv.beginBitmapFill(obst_bmp,mx,true);
                    mv._x=pos_pointer;mv._y=up_y+all_dist+movingObst_space;
                    mv['left_lim']=mv._y-all_dist;mv['right_lim']=mv.left_lim+all_dist;
                    mv.lineTo(movingObst_dist,0);
                    mv.lineTo(movingObst_dist,all_dist);
                    mv.lineTo(0,all_dist);
                    mv.endFill();
                    //
                    var rand_dist=random(all_dist);
                    mc._y-=rand_dist;mv._y-=rand_dist;
                    //
                    var moving_sense=random(2)*2-1;mc['step']=step*moving_sense;mv['step']=step*moving_sense;
                    mc.onEnterFrame=move_obst;mv.onEnterFrame=move_obst;
                    //
                    pos_pointer+=movingObst_dist+(inter_obst/2);
                }
            }
        }
        function move_obst(){
            this._y+=this.step;
            var limit_bool=this.step<0?this._y<this.left_lim:this.right_lim<this._y;
            if(limit_bool==true)this.step*=-1;
        }
        ep.loader_fn=add_obst;
        ep.new_zone();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"obstacles");

    //slows
    int slows_small_lat=2*ship_body_sz[0];
    int inter_slows=inter_obst-ship_body_sz[0];
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var objects_roadStart=%u;var long_lat=%u",items_roadStart,latura_mare);
    actionf_sprite(presprite,buf,"var slowers_small_lat=%u;var inter_slowers=%u",slows_small_lat,inter_slows);
    actionf_sprite(presprite,buf,"var up_y=%u;var bottom_y=%u",items_defaultUp,slows_defaultBottom);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var slowers=ep.env_container.objects.slowers;
        var off=10;
        var long_half=(bottom_y-up_y)/2;
        var yPos;var xPos;
        function add_slowers(road){
            var pos_pointer=objects_roadStart;
            var factor=random(2)*2-1;
            //
            while(pos_pointer<long_lat){
                var d=slowers.getNextHighestDepth();var mc=slowers.createEmptyMovieClip('slow'+d,d);
                mc._x=road._x;mc._y=road._y;mc._rotation=road._rotation;
                //
                mc.beginFill(0,50);
                //
                yPos=up_y*factor;
                mc.moveTo(pos_pointer,yPos);
                //
                xPos=pos_pointer+slowers_small_lat;
                mc.curveTo(pos_pointer+(slowers_small_lat/2),yPos+(off*factor),xPos,yPos);
                //
                mc.curveTo(xPos-off,yPos+(long_half/2*factor),xPos,yPos+(long_half*factor));
                yPos+=long_half*factor;
                mc.curveTo(xPos-off,yPos+(long_half/2*factor),xPos,yPos+(long_half*factor));
                //
                yPos+=long_half*factor;
                mc.curveTo(pos_pointer+(slowers_small_lat/2),yPos-(off*factor),pos_pointer,yPos);
                //
                mc.curveTo(pos_pointer+off,yPos-(long_half/2*factor),pos_pointer,yPos-(long_half*factor));
                yPos-=long_half*factor;
                mc.curveTo(pos_pointer+off,yPos-(long_half/2*factor),pos_pointer,yPos-(long_half*factor));
                mc.endFill();
                //
                factor*=-1;pos_pointer=xPos+inter_slowers;
            }
        }
        ep.loader_fn=add_slowers;
        ep.new_zone();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"slowers");

    //fasters
    int faster_lat=ship_body_sz[0]/2;
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var objects_roadStart=%u;var long_lat=%u",items_roadStart,latura_mare);
    actionf_sprite(presprite,buf,"var faster_lat=%u",faster_lat);
    actionf_sprite(presprite,buf,"var up_y=%u;var bottom_y=%u",items_defaultUp,latura_mica_jumate-faster_lat);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var fasters=ep.env_container.objects.fasters;
        var inter_dist=faster_lat/2;
        function add_fasters(road){
            var pos_pointer=objects_roadStart;
            var yPos=up_y+random(bottom_y-up_y);var sense=random(2)*2-1;
            while(pos_pointer<long_lat){
                var d=fasters.getNextHighestDepth();var mc=fasters.createEmptyMovieClip('fast'+d,d);
                mc._x=road._x;mc._y=road._y;mc._rotation=road._rotation;
                //
                mc.lineStyle(2,0x00ff00);
                mc.moveTo(pos_pointer,yPos);
                mc.lineTo(pos_pointer+faster_lat,yPos+(faster_lat/2));
                mc.lineTo(pos_pointer,yPos+faster_lat);
                //
                var test=yPos+(inter_dist*sense);
                if(test<up_y)sense*=-1;
                else if(bottom_y<test)sense*=-1;
                yPos+=inter_dist*sense;
                //
                pos_pointer+=faster_lat+10;
            }
        }
        ep.loader_fn=add_fasters;
        ep.new_zone();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"fasters");

    //masina
    int driver_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/14/driver.dbl",driver_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(driver_sz[0]/2),0-(driver_sz[1]/2));
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"driver_body");
    //
    int cars_step=step/2;
    int dist_intreNave=4*ship_body_sz[0];
    int frames_intreNave=dist_intreNave/cars_step;
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var wd=%u;var hg=%u",driver_sz[0],driver_sz[1]);
    actionf_sprite(presprite,buf,"var frames_end=%u",fps);
    actionf_sprite(presprite,buf,"var long_lat=%u",latura_mare);
    action_sprite(presprite,R"(
        var com=_root['shared_level'];
        attachMovie('driver_body','body',getNextHighestDepth());
        com.append_fuel(wd,hg/2,'fuel',this);fuel._x=0-(wd/2);
        onEnterFrame=function(){
            var ep=_root['episode'];
            fuel.fuelOn();
            _x+=step;
            //
            ep.rot_0();
            var test=hitTest(road);
            ep.rot_back();
            //
            if(test==false){
                var nextRoad_bool=flag!=true?road_nr+1<ep.roads_array.length:road_nr-1>=0;
                if(nextRoad_bool==true){
                    road_nr=road_nr+(flag!=true?1:-1);
                    road=ep.roads_array[road_nr];
                    _parent._x=road._x;_parent._y=road._y;_parent._rotation=road._rotation;
                    _x=(flag!=true?0:long_lat);
                }else{
                    frames_end--;
                    if(frames_end<0)_parent.removeMovieClip();
                }
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"driver");
    //masini acelasi sens
    //masini contrasens
        //asta e aici e folosit la driver
    action("var roads_array");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var inter_ships=%u",dist_intreNave);
    actionf_sprite(presprite,buf,"var long_lat=%u;var small_half_lat=%u",latura_mare,latura_mica_jumate);
    actionf_sprite(presprite,buf,"var driver_lat=%u",driver_sz[0]);
    actionf_sprite(presprite,buf,"var up_y=%u;var bottom_y=%u",items_defaultUp,slows_defaultBottom);
    actionf_sprite(presprite,buf,"var step=%u",cars_step);
    actionf_sprite(presprite,buf,"var inter_frames=%u",frames_intreNave);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        if(flag==true){inter_ships*=2;inter_frames*=2;}
        var slowers=ep.env_container.objects.slowers;
        //
        var dist_all=bottom_y-up_y;
        var root_sense=random(2)*2-1;
        var sense=root_sense;
        ep.roads_array=new Array();var road_nr=0;
        //
        function add_shipsLat(xPos,senseVal,roadNumber){
            var roadMc=ep.roads_array[roadNumber];
            var dist=dist_all;
            var y_pos=small_half_lat*senseVal;
            dist-=driver_lat/2;y_pos+=driver_lat/2*(senseVal*-1);
            while(dist>0){
                var d=slowers.getNextHighestDepth();var mc=slowers.createEmptyMovieClip('driver'+d,d);
                mc._x=roadMc._x;mc._y=roadMc._y;mc._rotation=roadMc._rotation;
                //
                var mv=mc.attachMovie('driver','driver',mc.getNextHighestDepth());
                mv._x=xPos;mv._y=y_pos;
                mv['road']=roadMc;mv['road_nr']=roadNumber;
                if(flag!=true)mv['step']=step;
                else{mv['step']=0-step;mv._rotation=180;mv['flag']=flag;}
                //
                dist-=driver_lat;y_pos+=driver_lat*(senseVal*-1);
            }
        }
        var pos_pointer;var max_limit;var inter_dist;
        function add_ships(road){
            ep.roads_array.push(road);
            if(flag!=true){
                pos_pointer=0;
                while(pos_pointer<long_lat){
                    add_shipsLat(pos_pointer,sense,road_nr);
                    pos_pointer+=inter_ships;sense*=-1;
                }
            }
            else{
                pos_pointer=long_lat;
                while(0<pos_pointer){
                    add_shipsLat(pos_pointer,sense,road_nr);
                    pos_pointer+=0-inter_ships;sense*=-1;
                }
            }
            //
            road_nr++;
        }
        ep.loader_fn=add_ships;
        ep.new_zone();
        //
        var frames=inter_frames;
        var start_pos;
        onEnterFrame=function(){
            frames--;if(frames>0)return null;
            frames=inter_frames;
            root_sense*=-1;
            //
            if(flag!=true)add_shipsLat(0,root_sense,0);
            else add_shipsLat(long_lat,root_sense,ep.roads_array.length-1);
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"moving_ships");

    //tinte rosii
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var long_lat=%u;var small_half_lat=%u",latura_mare,latura_mica_jumate);
    actionf_sprite(presprite,buf,"var inter_rows=%u",target_sz[0]+(2*ship_body_sz[0]));
    actionf_sprite(presprite,buf,"var inter_targets=%u",target_sz[0]+ship_body_sz[0]);
    actionf_sprite(presprite,buf,"var target_sz=%u",target_sz[0]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var targets=ep.env_container.objects.targets;
        var yPos;
        function add_targets(road){
            var pos_pointer=0;
            while(pos_pointer<long_lat){
                yPos=(0-small_half_lat)+random(inter_targets);
                while(yPos<(small_half_lat-target_sz)){
                    var d=targets.getNextHighestDepth();var mc=targets.createEmptyMovieClip('target'+d,d);
                    mc._x=road._x;mc._y=road._y;mc._rotation=road._rotation;
                    var mv=mc.attachMovie('red_target','target',mc.getNextHighestDepth());
                    mv._x=pos_pointer;mv._y=yPos;
                    //
                    yPos+=inter_targets;
                }
                pos_pointer+=inter_rows;
            }
        }
        ep.loader_fn=add_targets;
        ep.new_zone();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"red_targets");

    //tinte verzi cu fasturi aruncate la hit
    int green_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/11/verde.dbl",green_sz);swf_sprite_placeobject(presprite,bmp,0);
    actionf_sprite(presprite,buf,"var lat=%u",ship_body_sz[0]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var fasters=ep.env_container.objects.fasters;
        function hitTest_function(){
            var rect=getBounds(fasters);
            var d=fasters.getNextHighestDepth();var mc=fasters.createEmptyMovieClip('fast'+d,d);
            mc._x=rect.xMin+((rect.xMax-rect.xMin)/2);mc._y=rect.yMin+((rect.yMax-rect.yMin)/2);
            mc._rotation=_parent._rotation;
            //
            mc.lineStyle(1,0x00ff00);
            var rows=4;var cols=4;var off=4;
            var yPos=rows/2*lat*-1;
            for(var row=0;row<rows;row++){
                var xPos=cols/2*lat*-1;
                for(var col=0;col<cols;col++){
                    mc.moveTo(xPos,yPos+off);
                    mc.lineTo(xPos+lat,yPos+(lat/2));
                    mc.lineTo(xPos,yPos+lat-off);
                    //
                    xPos+=lat;
                }
                yPos+=lat;
            }
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"green_target");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var long_lat=%u;var small_half_lat=%u",latura_mare,latura_mica_jumate);
    actionf_sprite(presprite,buf,"var inter_rows=%u;var lat=%u",green_sz[0]+(3*ship_body_sz[0]),green_sz[0]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var targets=ep.env_container.objects.targets;
        function add_targets(road){
            var pos_pointer=0;
            while(pos_pointer<long_lat){
                var d=targets.getNextHighestDepth();var mc=targets.createEmptyMovieClip('target'+d,d);
                mc._x=road._x;mc._y=road._y;mc._rotation=road._rotation;
                //
                var mv=mc.attachMovie('green_target','target',mc.getNextHighestDepth());
                mv._x=pos_pointer;mv._y=0-small_half_lat+random(2*small_half_lat-lat);
                //
                pos_pointer+=inter_rows;
            }
        }
        ep.loader_fn=add_targets;
        ep.new_zone();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"green_targets");

    //un tragator de la distanta de ocolit gloantele
    int shotMachine_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/14/shotMachine.dbl",shotMachine_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"shotMachine_body");
    //
    int gun_sz[2];
    bmp=swf_image_ex("C:/Desktop/universe/img/14/gun.dbl",gun_sz);
    presprite=swf_sprite_new();
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"shotMachine_gun");
    //
    presprite=swf_sprite_new();
    int guns=shotMachine_sz[1]/gun_sz[1];
    double sz_for_a_gun=double(shotMachine_sz[1])/guns;
    int bucata=(guns&1)==0?guns/2:(guns/2)+1;
    actionf_sprite(presprite,buf,"var shotMachine_h=%u;var gun_h=%u",shotMachine_sz[1],gun_sz[1]);
    actionf_sprite(presprite,buf,"var guns_nr=%u;var inter_guns=%lf",guns,sz_for_a_gun);
    actionf_sprite(presprite,buf,"var guns_active_nr=%u;var inter_frames=%u",bucata,2*fps);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        attachMovie('shotMachine_body','shotMachine_body',getNextHighestDepth());
        shotMachine_body._y=0-(shotMachine_h/2);
        var yPos=shotMachine_body._y;
        for(var i=0;i<guns_nr;i++){
            var d=getNextHighestDepth();
            var mc=attachMovie('shotMachine_gun','gun'+i,d);mc._y=yPos;
            yPos+=inter_guns;
        }
        //
        var frames=0;var sense_flag=random(2);
        var bullets=ep.env_container.objects.bullets;
        onEnterFrame=function(){
            frames--;if(frames>0)return null;
            frames=inter_frames;
            //
            sense_flag^=1;
            var sense=sense_flag*2-1;var gun_pos=sense==1?0:guns_nr-1;
            for(var guns=guns_active_nr;guns>0;guns--){
                var gun=this['gun'+gun_pos];gun_pos+=sense;
                d=bullets.getNextHighestDepth();
                mc=bullets.attachMovie('shotMachine_bullet','bullet'+d,d);
                mc._x=_parent._x;mc._y=_parent._y;mc._rotation=_parent._rotation;
                mc['xOff']=_x;mc['yOff']=gun._y;
                mc['rot']=_parent._rotation-180;
            }
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"shotMachine");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var step=%u",(10*ship_body_sz[0])/fps);
    actionf_sprite(presprite,buf,"var dist_limit=%u",latura_mare+latura_mica_jumate);
    actionf_sprite(presprite,buf,"var bullet_r=%u",gun_sz[1]/2);
	actionf_sprite(presprite, buf, R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        //
        beginFill(0xff8000);
        com.drawCircle_coords(this,bullet_r,xOff,yOff);
        endFill();
        //
        var dist=0;
        onEnterFrame=function(){
            if(dist>dist_limit)removeMovieClip();
            else{
                _x+=Math.cos(rot*Math.PI/180)*step;
                _y+=Math.sin(rot*Math.PI/180)*step;
                //
                dist+=step;
                //
                ep.rot_0();
				var a=hitTest(ep.ship.body);
				ep.rot_back();
                if(a==true){
					if(ep.ship.splash==undefined){
						ep.ship.attachMovie('splash','splash',ep.ship.getNextHighestDepth());
						ep.ship.splash.total_frames=%u;
					}
					ep.ship.splash.frames=ep.ship.splash.total_frames;
					removeMovieClip();
                }
            }
        }
    )", 3 * fps);
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"shotMachine_bullet");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var long_lat=%u;var small_half_lat=%u",latura_mare,latura_mica_jumate);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var slowers=ep.env_container.objects.slowers;
        function add_shoter(road){
            var d=slowers.getNextHighestDepth();
            var mc=slowers.createEmptyMovieClip('slow'+d,d);
            mc._x=road._x;mc._y=road._y;mc._rotation=road._rotation;
            var mv=mc.attachMovie('shotMachine','shotMachine',mc.getNextHighestDepth());
            mv._x=long_lat+(2*small_half_lat);
        }
        ep.loader_fn=add_shoter;
        ep.new_zone();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"shoter");

    //cercuri cam ca 12 penultima
    int cerc_sz[2];
    bmp=swf_image_ex("C:/Desktop/universe/img/12/platforma_in_zona.dbl",cerc_sz);
    int platform_diametru=2*latura_mica_jumate;
    int scala=100*platform_diametru/cerc_sz[0];
    int platforms_inter_dist=platform_diametru+(3*ship_body_sz[0]);
    int x_start=platform_diametru/2;
    //
    presprite=swf_sprite_new();
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(cerc_sz[0]/2),0-(cerc_sz[1]/2));
    //
    actionf_sprite(presprite,buf,"var step=%u*sense;var radius=%u;_xscale=%u;_yscale=%u",90/fps,platform_diametru/2,scala,scala);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var store_scale=_xscale;
        onEnterFrame=function(){
            ep.rot_0();
            if(hitTest(ep.ship.body)==true){
                //trebuie relativ de ep ca strada de _rotation!=0 da getBounds rotit
                var store_rot=_rotation;_rotation=0;
                _xscale=100;_yscale=100;
                //
                var rect=ep.ship.body.getBounds(ep);
                var this_rect=getBounds(ep);
                rect.xMin-=this_rect.xMin;rect.xMax-=this_rect.xMax;
                rect.yMin-=this_rect.yMin;rect.yMax-=this_rect.yMax;
                //
                _rotation=store_rot;
                _xscale=store_scale;_yscale=store_scale;
                //
                var xCenter=rect.xMin+((rect.xMax-rect.xMin)/2);
                var yCenter=rect.yMin+((rect.yMax-rect.yMin)/2);
                var dist=Math.sqrt((xCenter*xCenter)+(yCenter*yCenter));
                if(dist<=radius){
                    var angle=Math.atan2(yCenter,xCenter)*180/Math.PI;
                    angle+=step;
                    var newX=Math.cos(angle*Math.PI/180)*dist;
                    var newY=Math.sin(angle*Math.PI/180)*dist;
                    ep.env_container.objects._x-=newX-xCenter;ep.env_container.objects._y-=newY-yCenter;
                }
            }
            ep.rot_back();
            _rotation+=step;if(Math.abs(_rotation)>360)_rotation=_rotation%360;
        }
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"platform");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var long_lat=%u",latura_mare);
    actionf_sprite(presprite,buf,"var x_start=%u",x_start);
    actionf_sprite(presprite,buf,"var inter_dist=%u",platforms_inter_dist);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var platforms=ep.env_container.objects.platforms;
        function add_platforms(road){
            var pos_pointer=x_start;
            var sense=random(2)*2-1;
            while(pos_pointer<long_lat){
                var d=platforms.getNextHighestDepth();var mc=platforms.createEmptyMovieClip('platform'+d,d);
                mc._x=road._x;mc._y=road._y;mc._rotation=road._rotation;
                //
                var mv=mc.attachMovie('platform','platform',mc.getNextHighestDepth());
                mv._x=pos_pointer;
                mv['sense']=sense;
                //
                pos_pointer+=inter_dist;sense*=-1;
            }
        }
        ep.loader_fn=add_platforms;
        ep.new_zone();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"platforms");

    //

    actionf(buf,"var width=%u;var height=%u",width,height);
    actionf(buf,"var ship_body_h=%u",ship_body_sz[1]);
    actionf(buf,"var small_lat_half=%u;var long_lat=%u",latura_mica_jumate,latura_mare);
	//com.space_env(this,'spc',getNextHighestDepth());
	//spc._x = 0 - _x; spc._y = 0 - _y;
	action(R"(
        var level_y=0;var prev_level_y;
        var road_mc;
        function add_road(end_var,r){
			var c;
			if(r==-90){
				if(road_mc!=undefined)c=road_mc._rotation==0?-1:1;
			}else c=r==0?1:-1;
			var d=env_container.objects.roads.getNextHighestDepth();road_mc=env_container.objects.roads.attachMovie('road','road'+d,d);
			road_mc.inter=c;
			road_mc.end_line=end_var;
			road_mc._rotation=r;
		}
        var store_loader;var aux_function;
        function new_zone(){
			var random_sense=random(2)*2-1;
            prev_level_y=level_y;
            //
            add_road(undefined,-90);
            road_mc._y=level_y;
            loader_fn(road_mc);
            road_mc.onEnterFrame=function(){
                rot_0();
                if(this.hitTest(ship.body)==true){
                    store_loader.removeMovieClip();
                    aux_function();
                }
                rot_back();
            }
            //
            level_y-=long_lat+small_lat_half;
            add_road(undefined,random_sense==1?0:180);
            road_mc._y=level_y;
            road_mc._x=(0-small_lat_half)*random_sense;
            loader_fn(road_mc);
            //
            level_y+=small_lat_half;
            add_road(undefined,-90);
            road_mc._y=level_y;
            road_mc._x=long_lat*random_sense;
            loader_fn(road_mc);
            //
            level_y-=long_lat+small_lat_half;
            add_road(last_zone_flag,random_sense==1?180:0);
            road_mc._y=level_y;road_mc._x=(long_lat+small_lat_half)*random_sense;
            level_y+=small_lat_half;
            loader_fn(road_mc);
            road_mc.onEnterFrame=function(){
                rot_0();
                if(this.hitTest(ship.body)==true){
                    for(var obj_name in env_container.objects){
                        mv=env_container.objects[obj_name];
                        for(var mc_name in mv){
                            mc=mv[mc_name];
                            if(prev_level_y<mc._y)mc.removeMovieClip();
                        }
                    }
                    this.onEnterFrame=undefined;
                    set_zone();
                }
                rot_back();
            }
        }
        var back;
        function rot_0(){
            back=env_container._rotation;
            env_container._rotation=0;
        }
        function rot_back(){
            env_container._rotation=back;
        }

        createEmptyMovieClip('env_container',getNextHighestDepth());
		function stars_bg(onX,onY){
            var d=env_container.getNextHighestDepth();
            com.space_env(env_container,'b'+d,d);
            var mc=env_container['b'+d];mc._xscale=onX;mc._yscale=onY;
        }
        stars_bg(150,150);stars_bg(-150,-150);stars_bg(-150,150);stars_bg(150,-150);
        //
        env_container.createEmptyMovieClip('objects',env_container.getNextHighestDepth());
        env_container.objects.createEmptyMovieClip('roads',env_container.objects.getNextHighestDepth());
        env_container.objects.createEmptyMovieClip('solids',env_container.objects.getNextHighestDepth());
        env_container.objects.createEmptyMovieClip('slowers',env_container.objects.getNextHighestDepth());
        env_container.objects.createEmptyMovieClip('fasters',env_container.objects.getNextHighestDepth());
        env_container.objects.createEmptyMovieClip('targets',env_container.objects.getNextHighestDepth());
        env_container.objects.createEmptyMovieClip('bullets',env_container.objects.getNextHighestDepth());
        env_container.objects.createEmptyMovieClip('platforms',env_container.objects.getNextHighestDepth());
        attachMovie('ship','ship',getNextHighestDepth());
        _x=width/2;
        _y=height-(ship_body_h*3/2);
        //
        var zoneNr=0;
        var loader;var last_zone_flag;
        function set_zone(){
            store_loader=loader;
            if(zoneNr==0)loader=attachMovie('obstacles','obstacles',getNextHighestDepth());
            else if(zoneNr==1)loader=attachMovie('slowers','slowers',getNextHighestDepth());
            else if(zoneNr==2)loader=attachMovie('fasters','fasters',getNextHighestDepth());
            else if(zoneNr==3)loader=attachMovie('moving_ships','moving_ships',getNextHighestDepth());
            else if(zoneNr==4)loader=attachMovie('red_targets','red_targets',getNextHighestDepth());
            else if(zoneNr==5)loader=attachMovie('green_targets','green_targets',getNextHighestDepth());
            else if(zoneNr==6)loader=attachMovie('shoter','shoter',getNextHighestDepth());
            else if(zoneNr==7)loader=attachMovie('platforms','platforms',getNextHighestDepth());
            else if(zoneNr==8){
                loader=attachMovie('moving_ships','moving_ships_flag',getNextHighestDepth());
                loader['flag']=true;
            }else if(zoneNr==9){
				last_zone_flag=true;
                loader=attachMovie('obstacles','obstacles_flag',getNextHighestDepth());
                loader['flag']=true;
            }else{
                aux_function=end_function;
                loader_fn=undefined;
                new_zone();
            }
            zoneNr++;
        }
        function end_function(){
            _root.end_scenario();
        }
        set_zone();
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
