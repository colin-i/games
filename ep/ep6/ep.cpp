
#include<windows.h>
#include<stdio.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

int _start(){
    char* ep_nm=Asteroids_ep;
	sprintf(buf,"%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);
    action("_root['episode']=this");
    init_episod(ep_nm);

    actionf(buf,"var width=%u;var height=%u;var fps=%u",width,height,fps);

    //ship

    //gun and bullet

    int ship_body_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/6/ship_texture.dbl",ship_body_sz);
    swf_exports_add(dbl,"ship_texture");
    //
    int normal_gun_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/6/normal_gun.dbl",normal_gun_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(normal_gun_sz[0]/2),0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"normal_gun");
        //
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        ep.set_bullet(this,10,ep.ship._rotation,1,0xff6000,0,false);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"normal_bullet");
    //
    int auto_gun_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/6/auto_gun.dbl",auto_gun_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(auto_gun_sz[0]/2),0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"auto_gun");
        //
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        ep.set_bullet(this,10,ep.ship._rotation,1,0xff9000,0,false);
        ep.bonus_expire();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"auto_bullet");
    //
    int persistent_gun_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/6/persistent_gun.dbl",persistent_gun_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(persistent_gun_sz[0]/2),0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"persistent_gun");
        //
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        ep.set_bullet(this,20,ep.ship._rotation,4,0xff0000,0,true);
        ep.bonus_expire();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"persistent_bullet");
    //
    int round_gun_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/6/round_gun.dbl",round_gun_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(round_gun_sz[0]/2),0-(round_gun_sz[1]/2));
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"round_gun");
        //
    action("var round_bullet_total=4;var round_bullet_nr=round_bullet_total");
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var bullet_off=%u",round_gun_sz[0]/2);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        ep.set_bullet(this,10,ep.ship._rotation+(360*ep.round_bullet_nr/ep.round_bullet_total),1,0xff3000,bullet_off,false);
        ep.round_bullet_nr--;
        if(ep.round_bullet_nr>0)ep.ship.add_one_bullet();
        else{ep.round_bullet_nr=ep.round_bullet_total;ep.bonus_expire();}
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"round_bullet");

    //ship body

    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var ship_body_w=%u;var ship_body_h=%u",ship_body_sz[0],ship_body_sz[1]);
    action_sprite(presprite,R"(
        var ship_texture=flash.display.BitmapData.loadBitmap('ship_texture');
        var ship_matrix=new flash.geom.Matrix();
        var half_w_sz=ship_body_w/2;
        beginBitmapFill(ship_texture,ship_matrix,true);
        lineTo(0-half_w_sz,ship_body_h);
        lineTo(half_w_sz,ship_body_h);
        lineTo(0,0);
        endFill();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_body");

    //the ship

    presprite=swf_sprite_new();
	actionf_sprite(presprite, buf, "var height=%u;var fps=%u;var shot_interval=%u", height, fps,shot_interval);
    actionf_sprite(presprite,buf,"var ship_body_w=%u;var ship_body_h=%u",ship_body_sz[0],ship_body_sz[1]);
	automatic(1);
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        //
        var ship_body_half_w=ship_body_w/2;var ship_body_half_h=ship_body_h/2;
        //
        com.add_fuel(ship_body_w,ship_body_half_h,1,'backFuel',this);backFuel._x=0-ship_body_half_w;
        //
        var front_fuel_w=ship_body_w/2;
        com.add_fuel(front_fuel_w,ship_body_h,-1,'frontLeftFuel',this);frontLeftFuel._x=0-ship_body_half_w;
        com.add_fuel(front_fuel_w,ship_body_h,-1,'frontRightFuel',this);frontRightFuel._x=ship_body_half_w-front_fuel_w;
        //
        backFuel._y=ship_body_half_h;frontLeftFuel._y=ship_body_half_h;frontRightFuel._y=ship_body_half_h;
        //
        attachMovie('ship_body','ship_body',getNextHighestDepth());
        ship_body._y=0-ship_body_half_h;
        //
        attachMovie('shields','shields',getNextHighestDepth());
        //
        var attack_type;var shot_interval;
        function normal_attack(){
            set_attack('normal',shot_interval);
        }
        function set_attack(type,interval){
            attack_type=type;shot_interval=interval;
            attachMovie(attack_type+'_gun',attack_type+'_gun',getNextHighestDepth());
        }
        function remove_attack(){
            this[attack_type+'_gun'].removeMovieClip();
        }
        normal_attack();
        //
        var step_max=ship_body_h;var frames_to_stepMax=2*fps;
        var y_step_max=step_max;var x_step_max=0;
        var x_step=0;var y_step=0;
        //
        ep.bullets_step_max=step_max+10;
        //
        var rotation_speed=180/fps;
		function shot_func(){
			if(damage_taken==undefined){
				if(shot_timer==undefined){
					add_one_bullet();
                    createEmptyMovieClip('shot_timer',getNextHighestDepth());
                    shot_timer['shot_interval']=shot_interval;
                    shot_timer.onEnterFrame=function(){
						this.shot_interval--;if(this.shot_interval<0)this.removeMovieClip();
                    }
                }
            }
		}
        onEnterFrame=function(){
            if(Key.isDown(com.action_key)==true)shot_func();else if(ep.auto_shot==true)shot_func();
            if(Key.isDown(com.up_arrow)==true){
                backFuel.fuelOn();
                add_speed(x_step_max,y_step_max);
            }
            else if(Key.isDown(com.down_arrow)==true){
                frontLeftFuel.fuelOn();
                frontRightFuel.fuelOn();
                add_speed(0-x_step_max,0-y_step_max);
            }
            else if(x_step!=0)slow_ship();
            else if(y_step!=0)slow_ship();
            if(Key.isDown(com.left_arrow)==true){
                _rotation-=rotation_speed;
                y_step_max=Math.cos(_rotation*Math.PI/180)*step_max;
                x_step_max=Math.sin((_rotation-180)*Math.PI/180)*step_max;
            }
            else if(Key.isDown(com.right_arrow)==true){
                _rotation+=rotation_speed;
                y_step_max=Math.cos(_rotation*Math.PI/180)*step_max;
                x_step_max=Math.sin((_rotation-180)*Math.PI/180)*step_max;
            }
        }
        Key.addListener(this);
        onKeyUp=function(){
            if(Key.getCode()==com.up_arrow){
                backFuel.clear();
            }
            else if(Key.getCode()==com.down_arrow){
                frontLeftFuel.clear();frontRightFuel.clear();
            }
        }
        function add_speed(xsMax,ysMax){
            x_step=step_decision(x_step,xsMax,xsMax/frames_to_stepMax);
            y_step=step_decision(y_step,ysMax,ysMax/frames_to_stepMax);
        }
        //resulting_add
        function step_decision(current_step,target_step,add_nr){
            var dist_before=Math.abs(target_step-current_step);
            current_step+=add_nr;
            var dist_after=Math.abs(target_step-current_step);
            if(dist_after>dist_before)current_step-=2*add_nr;
            return current_step;
        }
        function slow_ship(){
            x_step-=x_step/frames_to_stepMax;
            y_step-=y_step/frames_to_stepMax;
            if(Math.abs(x_step)<1)if(Math.abs(y_step)<1){x_step=0;y_step=0;}
        }
        function add_one_bullet(){
            var d=ep.bullets.getNextHighestDepth();ep.bullets.attachMovie(attack_type+'_bullet','bullet'+d,d);
        }

    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship");

    //shields

    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var ship_body_half_lat=%u",ship_body_sz[0]/2);
    action_sprite(presprite,R"(
        var com=_root['shared_level'];
        var shield_sz=2;
        var main_off=ship_body_half_lat+(shield_sz/2)+1;var inter_off=shield_sz+(shield_sz/2)+2;
        function draw_shield(pos){
            var mc=createEmptyMovieClip('shield'+pos,pos);
            mc.lineStyle(shield_sz,0x3300FF,70);
            var r=main_off+(pos*inter_off);
            com.drawCircle_offset(mc,r,0);
            mc._x=0-r;mc._y=0-r;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"shields");

    //damage taken
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var ship_body_lat=%u",ship_body_sz[0]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        var dt_matrix=new flash.geom.Matrix();
        var colors=new Array(0xff0000,0xffFF00);
        //
        var alphas=new Array(80,80);
        var ratios=new Array(0,255*ship_body_lat/Stage.width);
        var alpha_decrement=100/(ep.fps*3);
        onEnterFrame=function(){
            dt_alpha-=alpha_decrement;
            if(dt_alpha<0)removeMovieClip();
            else{
                alphas[0]=dt_alpha;alphas[1]=dt_alpha;
                clear();
                beginGradientFill('radial',colors,alphas,ratios,dt_matrix);
                var r=ship_body_lat;
                com.drawCircle_offset(this,r,0-r);
                endFill();
            }
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"damage_taken");

    //background
    dbl=swf_dbl("C:/Desktop/universe/img/6/fog.dbl");
    swf_exports_add(dbl,"fog_dbl");
    action("var fog_bmp=flash.display.BitmapData.loadBitmap('fog_dbl');");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var matrix=new flash.geom.Matrix();
        beginBitmapFill(ep.fog_bmp,matrix,true);
        lineTo(width,0);lineTo(width,height);
        lineTo(0,height);lineTo(0,0);
        endFill();
        _alpha=40;
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"fog");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u",width,height);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        //var com=_root['shared_level'];
        //
        var leftTop;var rightTop;var leftBottom;var rightBottom;

        var d=getNextHighestDepth();var n='sp_env_'+d;
        //com.space_env(this,n,d);
        attachMovie('fog',n,d);
        leftTop=this[n];
        d=getNextHighestDepth();n='sp_env_'+d;
        //com.space_env(this,n,d);
        attachMovie('fog',n,d);
        rightTop=this[n];rightTop._x=width;
        d=getNextHighestDepth();n='sp_env_'+d;
        //com.space_env(this,n,d);
        attachMovie('fog',n,d);
        rightBottom=this[n];rightBottom._x=width;rightBottom._y=height;
        d=getNextHighestDepth();n='sp_env_'+d;
        //com.space_env(this,n,d);
        attachMovie('fog',n,d);
        leftBottom=this[n];leftBottom._y=height;

        function zones_move(){
            _x+=ep.ship.x_step;_y+=ep.ship.y_step;
            if(_x>0){
                replace_zone('rightTop','leftTop');
                replace_zone('rightBottom','leftBottom');
                _x-=width;
            }
            else if(_x<(0-width)){
                replace_zone('leftTop','rightTop');
                replace_zone('leftBottom','rightBottom');
                _x+=width;
            }
            if(_y>0){
                replace_zone('leftBottom','leftTop');
                replace_zone('rightBottom','rightTop');
                _y-=height;
            }
            else if(_y<(0-height)){
                replace_zone('leftTop','leftBottom');
                replace_zone('rightTop','rightBottom');
                _y+=height;
            }
        }
        function replace_zone(outzone_name,newzone_name){
            var zone=this[outzone_name];var movingzone=this[newzone_name];
            var d=zone.getDepth();var n=zone._name;
            var out_x=zone._x;var out_y=zone._y;
            zone.removeMovieClip();
            //com.space_env(this,n,d);
            attachMovie('fog',n,d);
            this[newzone_name]=this[n];this[newzone_name]._x=movingzone._x;this[newzone_name]._y=movingzone._y;
            this[outzone_name]=movingzone;this[outzone_name]._x=out_x;this[outzone_name]._y=out_y;
        }
        onEnterFrame=zones_move
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"zones");

    //bullet
    action(R"(
        function set_bullet(bullet,size,rot,line_sz,color,off,persistence){
            bullet.lineStyle(line_sz,color);
            bullet.moveTo(0,0-off);
            bullet.lineTo(0,0-off-size);
            bullet._rotation=rot;
            bullet._x=ship._x;bullet._y=ship._y;
            bullet.x_step=Math.sin(rot*Math.PI/180)*bullets_step_max;
            bullet.y_step=0-Math.cos(rot*Math.PI/180)*bullets_step_max;
            bullet.persistence=persistence;
            //
            bullet.onEnterFrame=function(){
                var clips=asteroids.clips;
                for(var item in clips){
                    if(this.hitTest(clips[item])==true){
                        _root.counterBar_step();
                        //ok =
                        if(clips[item]._xscale>=100)asteroids.split_asteroid(clips[item]);
                        clips[item].removeMovieClip();
                        //la flash mai continua dupa removeMovieClip cu this=undefined
                        //e ok return atunci
                        if(this.persistence==false)return this.removeMovieClip();
                    }
                }
                move_in_zones_obj(this);
            }
        }
    )");

    //asteroids
    int asteroid_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/3/rock0.dbl",asteroid_sz);
    swf_exports_add(dbl,"rock0");
    dbl=swf_dbl("C:/Desktop/universe/img/6/rock1.dbl");
    swf_exports_add(dbl,"rock1");

    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var asteroid_in_frames=ep.fps;var asteroid_in_frames_index=asteroid_in_frames;
        createEmptyMovieClip('clips',getNextHighestDepth());
        var min_scale=80;var max_scale=min_scale+100;
        var scale_increment=40;
	var maximum_asteroid_upgrades=scale_increment*10+min_scale;
        function add_asteroids_size(){
	    if(min_scale<maximum_asteroid_upgrades){
		min_scale+=scale_increment;max_scale+=scale_increment;
	    }
        }
        function add_asteroid(){
            asteroid_in_frames_index--;
            if(asteroid_in_frames_index!=0)return null;
            asteroid_in_frames_index=asteroid_in_frames;
            var a=insert_asteroid();
            var scale=min_scale+random(max_scale-min_scale);
            a._xscale=scale;a._yscale=scale;
        }
        //asteroid
        function insert_asteroid(insert_on_place){
            var d=0;
            while(clips.getInstanceAtDepth(d)!=undefined)d++;
            var n='asteroid'+d;
            var mc=clips.attachMovie('asteroid',n,d);
            mc['insert_on_place']=insert_on_place;
            return mc;
        }
        function split_asteroid(ast){
            var new_scale=ast._xscale/2;
            var ast_x=ast._x;var new_y=ast._y+(ast._height/2);
            var ast_w=ast._width;
            var angleRadians=Math.atan2(ast.y_step,ast.x_step);var angle=angleRadians*180/Math.PI;var radius=Math.sqrt((ast.x_step*ast.x_step)+(ast.y_step*ast.y_step));
            var ast_x_step=ast.x_step;
            //
            var asteroid_one=insert_asteroid(true);
            var asteroid_two=insert_asteroid(true);
            //scale
            asteroid_one._xscale=new_scale;asteroid_one._yscale=new_scale;
            asteroid_two._xscale=new_scale;asteroid_two._yscale=new_scale;
            //step
            var min_dif=5;
            var one_angle=angle+random(90-min_dif);
            asteroid_one.x_step=Math.cos(one_angle*Math.PI/180)*radius;asteroid_one.y_step=Math.sin(one_angle*Math.PI/180)*radius;
            var two_angle=angle-random(90-min_dif);
            asteroid_two.x_step=Math.cos(two_angle*Math.PI/180)*radius;asteroid_two.y_step=Math.sin(two_angle*Math.PI/180)*radius;
            //x,y
            if(asteroid_one.x_step<asteroid_two.x_step){asteroid_one._x=ast_x;asteroid_two._x=ast_x+(ast_w/2);}
            else{asteroid_one._x=ast_x+(ast_w/2);asteroid_two._x=ast_x;}
            asteroid_one._y=new_y;asteroid_two._y=new_y;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"asteroids");

    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fps=%u;var asteroid_w=%u",fps,asteroid_sz[0]);
    action_sprite(presprite,R"(
        var com=_root['shared_level'];
        var ep=_root['episode'];

        if(insert_on_place!=true)ep.set_object(this,asteroid_w*_xscale/100);

        var bmp_pre=flash.display.BitmapData.loadBitmap('rock'+random(2));
        var rotation_step=(90/fps)*x_step/ep.obj_step_max;
        //show ca sa calculeze la set_object cu _width si _height
        com.mc_rotation_bmp_show(bmp_pre,this,'rock',0,rotation_step);
        this['rock'].onEnterFrame=com.rotate_bmp;

        //
        var ship_hit_stepDivide=2;var cease_damage_frames=0;
        onEnterFrame=function(){
            if(cease_damage_frames!=0)cease_damage_frames--;
			else if(hitTest(ep.ship.ship_body)==true){
				cease_damage_frames=fps;
                //
                var last_shield=ep.ship.shields.getInstanceAtDepth(ep.ship.shields.getNextHighestDepth()-1);
                if(last_shield!=undefined)last_shield.removeMovieClip();
                else{
					if(ep.ship.damage_taken==undefined)ep.ship.attachMovie('damage_taken','damage_taken',ep.ship.getNextHighestDepth());
                    ep.ship.damage_taken.dt_alpha=90;
                    //
                    ep.ship.x_step/=ship_hit_stepDivide;ep.ship.y_step/=ship_hit_stepDivide;
                }
            }
            ep.move_in_zones_obj(this);
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"asteroid");

    //bonuses
    action(R"(
    function random_BonusChance(arr){
        var i=0;nr;max=-1;
        while(i<arr.length){
            var b_nr=arr[i].chance;
            var max_candidate=random(b_nr);
            if(max_candidate>max){nr=i;max=max_candidate;}
            i++;
        }
        var splits=arr[nr].chance;arr[nr].chance=0;
        var parts=arr.length-1;
        var equal_splits=int(splits/parts);var rest_at=random(parts);
        var i=0;while(i<arr.length){
            if(i!=nr){
                if(rest_at==0)arr[i].chance+=splits%parts;
                rest_at--;
                arr[i].chance+=equal_splits;
            }
            i++;
        }
        return nr;
    }

    function Bonus(type,color,chance,interval){
        this.type=type;this.color=color;this.chance=chance;this.interval=interval;
    }
    var a1=new Bonus('auto',0x0000FF,10,1);var a2=new Bonus('persistent',0x00ff00,10,fps/3);var a3=new Bonus('round',0xffFF00,10,fps/3);
    var bonus_attack_type=new Array(a1,a2,a3);
    var shield=new Bonus('shield',0xff0000,10,3);
    var bonus_defense_type=new Array(shield);
    //
    function add_bonus(){
        if(bonus_layer.bonus==undefined){
            bonus_layer.attachMovie('bonus','bonus',0);

            var bonus_types=bonus_attack_type.slice();
			var shields=ship.shields;
			if(shields.getInstanceAtDepth(shields.getNextHighestDepth()-1)==undefined)bonus_types=bonus_types.concat(bonus_defense_type);

            bonus_layer.bonus.sel_bonus=bonus_types[random_BonusChance(bonus_types)];
        }
    }
    function bonus_expire(){
        if(bonus_bullets_sum!=0){
            bonus_bullets_sum--;
            if(bonus_bullets_sum==0){ship.remove_attack();ship.normal_attack();}
        }
    }
    )");
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var com=_root['shared_level'];
        var ep=_root['episode'];

        var bonus_w=20;
        var sz=5;
        beginFill(0x00ff00,90);
        var main_circle_off=sz+2;var main_circle_r=bonus_w/2-main_circle_off;
        com.drawCircle_offset(this,main_circle_r,main_circle_off);
        endFill();
        //
        lineStyle(sz,sel_bonus.color,90);var color_circle_off=sz/2;
        com.drawCircle_offset(this,bonus_w/2-color_circle_off,color_circle_off);
        //
        ep.set_object(this,_width);
        //
        onEnterFrame=function(){
            if(hitTest(ep.ship.ship_body)==true){
                if(sel_bonus.type!='shield'){
                    ep.ship.remove_attack();
                    ep.ship.set_attack(sel_bonus.type,sel_bonus.interval);
                    //zece secunde de tras non-stop gloante
                    ep.bonus_bullets_sum=int(ep.fps/sel_bonus.interval*10);
                }
                else{
                    var i=0;while(i<sel_bonus.interval){
                        if(ep.ship.shields.getInstanceAtDepth(i)==undefined)ep.ship.shields.draw_shield(i);
                        i++;
                    }
                }
                removeMovieClip();
            }
            else ep.move_in_zones_obj(this);
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bonus");

    actionf(buf,"var ship_body_w=%u;var ship_body_h=%u",ship_body_sz[0],ship_body_sz[1]);
    actionf(buf,"var obj_step_min=%u;var obj_step_max=%u",2*asteroid_sz[0]/fps,3*asteroid_sz[0]/fps);

    action(R"(
        com.space_env(this,'bg',getNextHighestDepth());
        attachMovie('zones','zones',getNextHighestDepth());
        attachMovie('ship','ship',getNextHighestDepth());
        ship._x=(width-ship_body_w)/2;ship._y=(height-ship_body_h)/2;
        var bullets_step_max;var bonus_bullets_sum=0;
        createEmptyMovieClip('bullets',getNextHighestDepth());
        attachMovie('asteroids','asteroids',getNextHighestDepth());
        createEmptyMovieClip('bonus_layer',getNextHighestDepth());
        var area_frames=30*fps;
        var area_frames_counter=area_frames;
        _root.counterBar_init(200);
        //
        onEnterFrame=function(){
            area_frames_counter--;if(area_frames_counter==0){
                area_frames_counter=area_frames;
                asteroids.add_asteroids_size();
            }
            asteroids.add_asteroid();
            add_bonus();
        }
        function move_in_zones_obj(obj){
            obj._x+=obj.x_step+ship.x_step;obj._y+=obj.y_step+ship.y_step;
            if(obj._x<(0-width))obj.removeMovieClip();
            else if(obj._x>(2*width))obj.removeMovieClip();
            else if(obj._y<(0-height))obj.removeMovieClip();
            else if(obj._y>(2*height))obj.removeMovieClip();
        }
        function set_object(obj,lat){
            var step_one=obj_step_min+random(obj_step_max-obj_step_min);
            var step_two=((random(200)-100)/100)*step_one;
            if(random(2)==0){
                obj._y=random(height);
                if(random(2)==0){
                    obj.x_step=step_one;obj.y_step=step_two;
                    obj._x=0-lat;
                }
                else{
                    obj.x_step=0-step_one;obj.y_step=step_two;
                    obj._x=width;
                }
            }
            else{
                obj._x=random(width);
                if(random(2)==0){
                    obj.y_step=step_one;obj.x_step=step_two;
                    obj._y=0-lat;
                }
                else{
                    obj.y_step=0-step_one;obj.x_step=step_two;
                    obj._y=height;
                }
            }
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
