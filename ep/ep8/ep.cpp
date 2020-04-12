
#include<windows.h>
#include<stdio.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

int _start(){
    char* ep_nm=Moving_Forward_ep;
	sprintf(buf, "%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);
    action("_root['episode']=this");
    init_episod(ep_nm);

    //ship_body, cel care face hitTest
    presprite=swf_sprite_new();
    int ship_sz[2];
    bmp=swf_image_ex("C:/Desktop/universe/img/8/ship.dbl",ship_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_body");

    #define step_max 3*ship_sz[0]/fps
    //ship
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var ship_w=%u;var ship_h=%u;var fps=%u;var width=%u",ship_sz[0],ship_sz[1],fps,width);
    actionf_sprite(presprite,buf,"var step_max=%u",step_max);
    action_sprite(presprite,R"(
        var com=_root['shared_level'];
        var ep=_root['episode'];
        //
        var x_step=0;var y_step=0;
        var step_maximum=step_max;
        var step_increment=step_max/fps;
        var step_decrement=step_max/fps;
        //
        attachMovie('ship_body','ship_body',getNextHighestDepth());
        var center_x=ship_w/2;var center_y=ship_h/2;
        attachMovie('waves','rightWaves',getNextHighestDepth());
        rightWaves._x=center_x;rightWaves._y=center_y;
        attachMovie('waves','leftWaves',getNextHighestDepth());
        leftWaves._x=center_x;leftWaves._y=center_y;leftWaves._rotation=180;
        attachMovie('waves','bottomWaves',getNextHighestDepth());
        bottomWaves._x=center_x;bottomWaves._y=center_y;bottomWaves._rotation=90;
        onEnterFrame=function(){
            if(Key.isDown(com.left_arrow)==true){
                rightWaves.add_waves();if(x_step>(0-step_maximum))x_step-=step_increment;else x_step+=step_increment;
                set_x();
            }
            else if(Key.isDown(com.right_arrow)==true){
                leftWaves.add_waves();if(x_step<step_maximum)x_step+=step_increment;else x_step-=step_increment;
                set_x();
            }
            else if(x_step!=0){
                x_step-=(Math.abs(x_step)/x_step)*step_decrement;if(Math.abs(x_step)<1)x_step=0;
                set_x();
            }
            bottomWaves.add_waves();
            if(y_step<step_maximum)y_step+=step_increment;else y_step-=step_increment;
            //ma uit cu cat se schimba y step pentru decor
            var zone_val=ep.zone._y;
            //
            ep.zone._y+=y_step;
            //decidere step_maximum dar pentru tura urmatoare
            step_maximum=step_max;
            moving_lr_set(ep.zone.fastClips);
            for(var mc_name in ep.zone.fastClips){
                var mc=ep.zone.fastClips[mc_name];
                if(hitTest(mc)==true){step_maximum=step_max*2;break;}
            }
            moving_lr_set(ep.zone.slowClips);
            for(var mc_name in ep.zone.slowClips){
                var mc=ep.zone.slowClips[mc_name];
                if(hitTest(mc)==true){step_maximum=step_max/2;break;}
            }
            //
            for(var mc_name in ep.zone.tallClips){
                var mc=ep.zone.tallClips[mc_name];
                if(hitTest(mc)==true){
                    var clip_bounds_staticTest=mc.getBounds(ep);
    //e sistemul de fara hit test si <= e incorect ca daca ar fi = inainte ar fi hitTest
                    if((clip_bounds_staticTest.yMax-y_step)<_y){ep.zone._y-=y_step;y_step=0;}
    // si la stanga si la dreapta e acelasi lucru
                    else _x-=x_step;
                    break;
                }
            }
            // aicea mut obstacolele ca sa calculez exact
            if(ep.zone.left_right_moving==true){
                for(var mc_name in ep.zone.tallClips){
                    var mc=ep.zone.tallClips[mc_name];
                    var x_move=moving_lr(mc);
                    if(hitTest(mc)==true)_x+=x_move;
                }
            }
            //
            zone_val=ep.zone._y-zone_val;
            ep.leftMargin.draw_margin(zone_val);ep.rightMargin.draw_margin(zone_val);
            ep.bg.set_bg(zone_val);
        }
        //x_move
        function moving_lr(mc){
            var x_move=mc.step*mc.sense;
            mc._x+=x_move;
            var clip_bounds=mc.getBounds(ep);
            if(clip_bounds.xMin<mc.min_x)mc.sense*=-1;
            else if(mc.max_x<clip_bounds.xMin)mc.sense*=-1;
            return x_move;
        }
        function moving_lr_set(zone_clips){
            if(ep.zone.left_right_moving==true){
                for(var mc_name in zone_clips)moving_lr(zone_clips[mc_name]);
            }
        }
        function set_x(){
            _x+=x_step;
            var left_x=ep.leftMargin._x+ep.leftMargin._width;
            if(_x<left_x)_x=left_x;
            var right_x=ep.rightMargin._x-ship_w;
            else if(right_x<_x)_x=right_x;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship");

    //waves
    action(R"(
        var wave_filter=new flash.filters.GlowFilter(0x00FFff,9/10,32,32,12);
        var wave_filterArray=new Array(wave_filter);
    )");
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fps=%u;var ship_w=%u",fps,ship_sz[0]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var wave_w=50;var wave_h=100;
        var waves_x_start=ship_w/2;
        var max_x=ep.waves_max;

        var wave_interval=fps/2;var wave_interval_frame=0;

        function add_waves(){
            wave_interval_frame++;if(wave_interval_frame!=wave_interval)return null;
            wave_interval_frame=0;
            //
            var d=ep.waves.getNextHighestDepth();
            var mc=ep.waves.createEmptyMovieClip('wave'+d,d);
            mc._x=ep.ship._x+_x;mc._y=ep.ship._y+_y;mc._rotation=_rotation;
            //
            mc.createEmptyMovieClip('maskMc',0);
            mc.maskMc.beginFill(0);
            mc.maskMc.moveTo(waves_x_start,-10);
            mc.maskMc.lineTo(waves_x_start,10);
            mc.maskMc.lineTo(max_x,wave_h/2);
            mc.maskMc.curveTo(max_x+wave_w,0,max_x,0-(wave_h/2));
            mc.maskMc.lineTo(waves_x_start,-10);
            mc.maskMc.endFill();
            mc.setMask(mc.maskMc);
            //
            var mv=mc.createEmptyMovieClip('drawMc',1);
            mv.lineStyle(2,0x00C0ff,80);
            //
            mv.curveTo(wave_w,wave_h/2,0,wave_h);
            mv.filters=ep.wave_filterArray;
            mv._y=0-(wave_h/2);
            mv.step=2*wave_w/fps;
            mv['wave_movie']=mc;
            mv.onEnterFrame=function(){
                this._x+=this.step;
                if(this._x>max_x)this.wave_movie.removeMovieClip();
            }
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"waves");

    //wood
    int wood_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/8/wood.dbl",wood_sz);
    swf_exports_add(dbl,"wood_texture");
    action("var wood_texture=flash.display.BitmapData.loadBitmap('wood_texture')");

    //margins
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var height=%u",height);
    actionf_sprite(presprite,buf,"var wood_h=%u",wood_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var margin_width=ep.margin_width;
        var margin_matrix=new flash.geom.Matrix();
        _x=x-(fix*margin_width);
        function draw_margin(distance){
            margin_matrix.ty+=distance;if(margin_matrix.ty>wood_h)margin_matrix.ty-=wood_h;
            //
            clear();
            beginBitmapFill(ep.wood_texture,margin_matrix,true);
            lineTo(margin_width,0);lineTo(margin_width,height);
            lineTo(0,height);lineTo(0,0);
            endFill();
        }
        //asta oricum din prima enterframe la inaintare apare
        //draw_margin(0);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"margin");

    //obstacles
    action("var obstacle_h=20");
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var obstacle_h=0-ep.obstacle_h;
        var mx=new flash.geom.Matrix();
        beginBitmapFill(ep.wood_texture,mx,true);
        moveTo(x_val,0);
        lineTo(xMax_val,0);lineTo(xMax_val,obstacle_h);
        lineTo(x_val,obstacle_h);lineTo(x_val,0);
        endFill();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"obstacle");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var fps=%u;var ship_w=%u",fps,ship_sz[0]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var clip_y=0;
        //
        var i=0;while(i<10){
            var d=tallClips.getNextHighestDepth();
            var mc1=tallClips.attachMovie('obstacle','obst'+d,d);d++;
            var mc2=tallClips.attachMovie('obstacle','obst'+d,d);
            clip_y=ep.rand_pass(mc1,mc2,clip_y);
            if(ep.zone.left_right_moving==true){
                var s=random(2);if(s==0)s=-1;
                ep.set_lr_parameters_1(mc1,mc2,s,ship_w/fps);
            }
            i++;
        }
        ep.zone_end(mc1);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"obstacles_zone");

    #define square_lat 3*ship_sz[0]

    //slow and fast
    dbl=swf_dbl("C:/Desktop/universe/img/8/slow_terrain.dbl");
    swf_exports_add(dbl,"slow_texture");
    action("var slow_texture=flash.display.BitmapData.loadBitmap('slow_texture')");
    //
    action("var fast_arrow");
    actionf(buf,"var fast_arrow_h=%u",square_lat/2);
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var l_size=2;var in_sz=l_size/2;
        var fast_arrow_h=ep.fast_arrow_h;
        var max_y=fast_arrow_h-in_sz;
        var margin_size=10;var arrow_sz=30;
        var mc=createEmptyMovieClip('clip',0);
        mc.lineStyle(l_size,0x008000);
        mc.beginFill(0x00FF00);
        mc.moveTo(0,in_sz);
        var x_to_go=ep.default_freeSpace/2-margin_size;
        mc.lineTo(x_to_go,50);mc.lineTo(x_to_go-arrow_sz,50);
        mc.lineTo(0,in_sz+arrow_sz);
        mc.lineTo(0-x_to_go+arrow_sz,50);mc.lineTo(0-x_to_go,50);
        mc.lineTo(0,in_sz);
        mc.endFill();
        mc._x=ep.default_freeSpace/2;
        //
        _root['episode'].fast_arrow=new flash.display.BitmapData(ep.default_freeSpace,fast_arrow_h,true,0);
        _root['episode'].fast_arrow.draw(this);
        removeMovieClip();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"fast_arrow");
    action("attachMovie('fast_arrow','fast_arrow_mv',getNextHighestDepth())");
    //
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var obstacle_h=0-item_h;
        var mx=new flash.geom.Matrix();
        beginBitmapFill(ep.slow_texture,mx,true);
        moveTo(x_val,0);
        lineTo(xMax_val,0);lineTo(xMax_val,obstacle_h);
        lineTo(x_val,obstacle_h);lineTo(x_val,0);
        endFill();
        _alpha=50;
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"slow");
    //
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];var mx=new flash.geom.Matrix();
        item_h*=-1;var fast_w=ep.default_freeSpace;
        if(w_mult!=undefined)fast_w*=w_mult;
        _x=x_val;
        beginBitmapFill(ep.fast_arrow,mx,true);
        lineTo(fast_w,0);lineTo(fast_w,item_h);lineTo(0,item_h);lineTo(0,0);
        endFill();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"fast");
    //
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var slow_fast_h=ep.fast_arrow_h*20;
        var i=0;var clip_y=0;
        //
        while(i<5){
            var d=slowClips.getNextHighestDepth();
            var mc1=slowClips.attachMovie('slow','slow'+d,d);d++;mc1['item_h']=slow_fast_h;
            var mc2=slowClips.attachMovie('slow','slow'+d,d);mc2['item_h']=slow_fast_h;
            d=fastClips.getNextHighestDepth();
            var mc_fast=fastClips.attachMovie('fast','fast'+d,d);mc_fast['item_h']=slow_fast_h;
            var clip_y=ep.rand_pass(mc1,mc2,clip_y,mc_fast);
            clip_y-=slow_fast_h;
            i++;
        }
        ep.zone_end(mc1);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"slow_and_fast_zone");

    //drum bun
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var square_lat=%u;var width=%u",square_lat,width);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var clip_y=0;
        var mc1;var mc2;var inter_mc;
        function prepare_slow(){
            var d=slowClips.getNextHighestDepth();
            mc1=slowClips.attachMovie('slow','slow'+d,d);d++;mc1['item_h']=square_lat;
            mc2=slowClips.attachMovie('slow','slow'+d,d);mc2['item_h']=square_lat;
            if(ep.zone.fastClips!=undefined){
                var d=fastClips.getNextHighestDepth();
                inter_mc=fastClips.attachMovie('fast','fast'+d,d);inter_mc['item_h']=square_lat;
            }
        }
        prepare_slow();
        ep.rand_pass(mc1,mc2,clip_y,inter_mc);
        clip_y-=square_lat;
        var road_x=mc1['xMax_val'];var left_right=new Array();
        var i=0;while(i<15){
            left_right.length=0;
            var at_left=road_x-square_lat;
            //aicea poate fi ca e random mai devreme
            if(ep.pass_x_min<=at_left)left_right.push(at_left);
            //aicea poate fi ca e random mai devreme
            if(road_x+(2*square_lat)<=(width-ep.pass_x_min))left_right.push(road_x);
            var new_x=left_right[random(left_right.length)];
            prepare_slow();
            mc1['x_val']=0;mc1['xMax_val']=new_x;mc1._y=clip_y;
            mc2['x_val']=new_x+(2*square_lat);mc2['xMax_val']=width;mc2._y=clip_y;
            if(ep.zone.fastClips!=undefined){
                inter_mc['x_val']=new_x;inter_mc._y=clip_y;inter_mc['item_h']*=2;inter_mc['w_mult']=2;
                mc1['item_h']*=2;mc2['item_h']*=2;
                clip_y-=square_lat;
            }
            clip_y-=square_lat;
            //
            if(new_x<road_x)road_x=new_x;else road_x=new_x+square_lat;
            //
            prepare_slow();
            mc1['x_val']=0;mc1['xMax_val']=road_x;mc1._y=clip_y;
            mc2['x_val']=road_x+square_lat;mc2['xMax_val']=width;mc2._y=clip_y;
            if(ep.zone.fastClips!=undefined){
                inter_mc['x_val']=road_x;inter_mc._y=clip_y;
            }
            clip_y-=square_lat;
            i++;
        }
        ep.zone_end(mc1);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"road_zone");

    //spiralat
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var square_lat=%u;var width=%u",square_lat,width);
    actionf_sprite(presprite,buf,"var obj_step=%u",step_max/3);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var x_val=ep.pass_x_min;var s=1;
        var right_limit=width-ep.pass_x_min-ep.default_freeSpace;
        var clip_y=0;
        //
        var i=0;while(i<40){
            var d=slowClips.getNextHighestDepth();
            var mc1=slowClips.attachMovie('slow','slow'+d,d);d++;mc1['item_h']=square_lat;
            var mc2=slowClips.attachMovie('slow','slow'+d,d);mc2['item_h']=square_lat;
            //
            mc1['xMax_val']=x_val;mc2['x_val']=x_val+ep.default_freeSpace;
            ep.set_lr_parameters_1(mc1,mc2,s,obj_step);
            mc1._y=clip_y;mc2._y=clip_y;
            //
            var next_x_val=x_val+(s*ep.default_freeSpace/2);
            if(right_limit<next_x_val){s=-1;x_val=right_limit;}
            else if(next_x_val<ep.pass_x_min){s=1;x_val=ep.pass_x_min;}
            else x_val+=s*ep.default_freeSpace/2;
            clip_y-=square_lat;
            i++;
        }
        ep.zone_end(mc1);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"zig_zag");

    //linii drepte step rapid / viteze
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u",width);
    actionf_sprite(presprite,buf,"var obj_step=%u",step_max);
    actionf_sprite(presprite,buf,"var square_lat=%u",square_lat);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        //se pune la dreapta, acelasi numar la stanga
        var nr=0;
        var x_val=ep.default_freeSpace;var xMax_val;
        var right_limit=width-ep.margin_width;
        //nr la dreapta
        while(x_val<right_limit){
            xMax_val=x_val+ep.default_freeSpace;
            x_val=xMax_val+ep.default_freeSpace;
            nr++;
        }
        //
        var dist_to_go=width-ep.default_freeSpace;var h_to_go;var row_nr;
        if(fastClips==undefined){h_to_go=40*ep.default_freeSpace;row_nr=1;}
        else{h_to_go=square_lat;row_nr=20;
            var clip_y=0;}
        nr*=2;var x_start=0-xMax_val;var mc;
        function obj_values_set(){
            var xMax_val=x_val+ep.default_freeSpace;
            mc['x_val']=x_val;mc['xMax_val']=xMax_val;mc['item_h']=h_to_go;
            mc['sense']=1;mc['min_x']=x_val;mc['max_x']=x_val+dist_to_go;
            return xMax_val;
        }
        function get_clip(a){
            a+=row_nr;
            if((a&1)==0){
                var d=fastClips.getNextHighestDepth();
                mc=fastClips.attachMovie('fast','fast'+d,d);
            }
            else{
                var d=slowClips.getNextHighestDepth();
                mc=slowClips.attachMovie('slow','slow'+d,d);
            }
            mc['step']=row_step;
            mc._y=clip_y;
        }
        while(row_nr>0){
            var i=nr;x_val=x_start;
            if(fastClips!=undefined){
                var row_step=obj_step/2+random(obj_step);
            }
            while(i>0){
                if(fastClips==undefined){
                    var d=slowClips.getNextHighestDepth();
                    mc=slowClips.attachMovie('slow','slow'+d,d);
                    mc['step']=obj_step;
                }
                else get_clip(0);
                //
                x_val=obj_values_set();
                //
                if(fastClips==undefined)x_val+=ep.default_freeSpace;
                else{
                    get_clip(1);
                    x_val=obj_values_set();
                }
                i--;
            }
            if(fastClips!=undefined)clip_y-=h_to_go+ep.default_freeSpace;
            row_nr--;
        }
        ep.zone_end(mc);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"offset_lr");

    //cutii

    int box_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/8/box.dbl",box_sz);
    swf_sprite_placeobject(presprite,bmp,0);swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"box");

    #define margin_width 10
    #define box_zone_w width-(2*margin_width)

    int box_combo_nr=box_zone_w;box_combo_nr/=2*box_sz[0];
    int combo_sz=int(double(box_zone_w)/box_combo_nr);
    int item_sz=int(double(box_zone_w)/box_combo_nr/2);
    int item_off=int((double(box_zone_w)/box_combo_nr/2-box_sz[0])/2);

    //cutii statice
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var box_combo_nr=%u;var combo_sz=%u;var item_sz=%u;var item_off=%u",box_combo_nr,combo_sz,item_sz,item_off);
    actionf_sprite(presprite,buf,"var fps=%u;var box_w=%u;var box_h=%u",fps,box_sz[0],box_sz[1]);
	action_sprite(presprite, R"(
        var ep=_root['episode'];
        var x_val;
        var clip_y=0-box_h;
        var row=0;
        //
        var step=box_w/fps;var xMin;var xMax;var yMax=box_h;var yMin=yMax-20;var mx=new flash.geom.Matrix();
        //
        while(row<15){
            x_val=ep.margin_width;var row_step=step/2+random(step);
            var i=0;while(i<box_combo_nr){
                var d=tallClips.getNextHighestDepth();
                var mc=tallClips.attachMovie('box','box'+d,d);
                mc._x=x_val;
                mc._x+=item_off;
                if(left_right_moving==true){
                    mc.min_x=mc._x;mc.max_x=mc._x+item_sz;
                    mc.sense=1;mc.step=row_step;
					var bol=i==0;if(bol==false)bol=(i+1)==box_combo_nr;
                    if(bol==true){
                        if(i==0){xMax=0;mc.min_x-=item_sz;mc.max_x-=item_sz;}
                        else xMax=box_w+item_sz;
                        xMin=xMax-item_sz;
                        mc.beginBitmapFill(ep.wood_texture,mx,true);
                        mc.moveTo(xMin,yMin);
                        mc.lineTo(xMax,yMin);mc.lineTo(xMax,yMax);
                        mc.lineTo(xMin,yMax);mc.lineTo(xMin,yMin);
                        mc.endFill();
                    }
                }
                if(row&1==0){
                    mc._x+=item_sz;
                }
                mc._y=clip_y;
                x_val+=combo_sz;
                i++;
            }
            clip_y-=2*box_h;
            row++;
        }
        ep.zone_end(mc);
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"box_zone");

    //bg
    int water_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/8/water_texture.dbl",water_sz);
    swf_exports_add(dbl,"water_texture");
    action("var bg_texture=flash.display.BitmapData.loadBitmap('water_texture')");
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u;var fps=%u",width,height,fps);
    actionf_sprite(presprite,buf,"var water_h=%u",water_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var bg_matrix=new flash.geom.Matrix();
        function set_bg(distance){
            bg_matrix.ty+=distance;if(bg_matrix.ty>water_h)bg_matrix.ty-=water_h;
        }
        var lr_w=20;
        var lr_step=lr_w/fps;var sense=1;
        onEnterFrame=function(){
            if(lr_w<bg_matrix.tx)sense*=-1;
            else if(bg_matrix.tx<0)sense*=-1;
            bg_matrix.tx+=lr_step*sense;
            //
            clear();
            beginBitmapFill(ep.bg_texture,bg_matrix,true);
            lineTo(width,0);lineTo(width,height);
            lineTo(0,height);lineTo(0,0);
            endFill();
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bg");

    //zones

    action(R"(
        var zoneLevel=0;
        function new_zone(){
            var d=zone.getDepth();zone.removeMovieClip();
            if(zoneLevel==0)obstacles_zone(d);
            else if(zoneLevel==1){
                attachMovie('slow_and_fast_zone','zone',d);
                slow_clips();fast_clips();
            }
            else if(zoneLevel==2)road_zone(d);
            else if(zoneLevel==3)box_zone(d);
            else if(zoneLevel==4){attachMovie('zig_zag','zone',d);slow_clips();zone.left_right_moving=true;}
            else if(zoneLevel==5){obstacles_zone(d);zone.left_right_moving=true;}
            else if(zoneLevel==6){road_zone(d);fast_clips();}
            else if(zoneLevel==7)offset_lr(d);
            else if(zoneLevel==8){box_zone(d);zone.left_right_moving=true;}
            else if(zoneLevel==9){offset_lr(d);fast_clips();}
            else _root.end_scenario();
            zoneLevel++;
        }
        function obstacles_zone(d){
            attachMovie('obstacles_zone','zone',d);
            tall_clips();
        }
        function road_zone(d){
            attachMovie('road_zone','zone',d);slow_clips();
        }
        function offset_lr(d){
            attachMovie('offset_lr','zone',d);slow_clips();
            zone.left_right_moving=true;
        }
        function box_zone(d){
            attachMovie('box_zone','zone',d);tall_clips();
        }
        //
        function tall_clips(){
            zone.createEmptyMovieClip('tallClips',zone.getNextHighestDepth());
        }
        function slow_clips(){
            zone.createEmptyMovieClip('slowClips',zone.getNextHighestDepth());
        }
        function fast_clips(){
            zone.createEmptyMovieClip('fastClips',zone.getNextHighestDepth());
        }
    )");

    actionf(buf,"var width=%u;var height=%u",width,height);
    actionf(buf,"var waves_max=%u",100-ship_sz[1]);
    actionf(buf,"var default_freeSpace=%u",ship_sz[0]*3);
    actionf(buf,"var ship_w=%u;var ship_h=%u",ship_sz[0],ship_sz[1]);
    actionf(buf,"var margin_width=%u",margin_width);

	action(R"(
        attachMovie('bg','bg',getNextHighestDepth());
        createEmptyMovieClip('waves',getNextHighestDepth());
        createEmptyMovieClip('zone',getNextHighestDepth());
        attachMovie('ship','ship',getNextHighestDepth());
        ship._x=(width-ship_w)/2;ship._y=height-waves_max-ship_h;
        attachMovie('margin','leftMargin',getNextHighestDepth());leftMargin['x']=0;leftMargin['fix']=0;
        attachMovie('margin','rightMargin',getNextHighestDepth());rightMargin['x']=width;rightMargin['fix']=1;
        new_zone();
        //
        var clips_margin=5;
        var pass_x_min=margin_width+clips_margin;
		var switcher=1;
        //clip_y-toNext
        function rand_pass(mc1,mc2,y_val,mc_inter){
            var freeSpace_randWidth=width-(2*margin_width)-(2*clips_margin)-default_freeSpace;
			freeSpace_randWidth/=3;
            var pass_x=pass_x_min+random(freeSpace_randWidth);
			if(switcher==1)pass_x+=freeSpace_randWidth+freeSpace_randWidth;switcher^=1;
            mc1['x_val']=0;mc1['xMax_val']=pass_x;
            mc2['x_val']=pass_x+default_freeSpace;mc2['xMax_val']=width;
            mc1._y=y_val;mc2._y=y_val;
            if(mc_inter!=undefined){mc_inter['x_val']=pass_x;mc_inter._y=y_val;}
            return y_val-500;
        }
        function zone_end(mc){
            zone.onEnterFrame=function(){
                var r=mc.getBounds(_root['episode']);
                if(r.yMin>height)new_zone();
            }
        }
        //
        function set_lr_parameters_1(mc1,mc2,sense,step){
            //se pune x_val si xMax_val, dar rapid
            mc1.x_val=mc1.xMax_val-width;mc2.xMax_val=mc2.x_val+width;
            //
            mc1['sense']=sense;mc2['sense']=sense;mc1['step']=step;mc2['step']=step;
            //se pune min_x max_x
            var val_min=mc1.xMax_val-pass_x_min;var val_max=width-pass_x_min-mc2.x_val;
            mc1.min_x=mc1.x_val-val_min;
            mc1.max_x=mc1.x_val+val_max;
            mc2.min_x=mc2.x_val-val_min;
            mc2.max_x=mc2.x_val+val_max;
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
