
#include<windows.h>
#include<stdio.h>

#include<math.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

int _start(){
    char* ep_nm=Jump_ep;
	sprintf(buf, "%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);
    action("_root['episode']=this");
    init_episod(ep_nm);

    //ground
    int ground_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/9/ground.dbl",ground_sz);
    swf_exports_add(dbl,"ground_object");
    action("var ground_object=flash.display.BitmapData.loadBitmap('ground_object')");
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var ground_object_h=%u",ground_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        //
        var landTest_function;
        //foloseste la ==this_mc , altfel this ar fi Global_as ca acolo e this la setTimeout
        var this_mc=this;
        if(soft_ground==true){
            function land_contact(){
                landTest_function=undefined;
                if(soft_extra_time!=undefined)setTimeout(extra_time,soft_extra_time);
                else normal_timeout();
            }
            function land_breaking(){
                clear();
                setTimeout(draw_land,2000);
                if(ep.jumper.mc_stand==this_mc)ep.jumper.start_falling();
            }
            function normal_timeout(){setTimeout(land_breaking,2000);}
            function extra_time(){
                clear();
                draw_mainLand();draw_unstableLand();
                normal_timeout();
            }
            function draw_unstableLand(){
                var yMin_rand=ground_object_h/4;var rand_sum=ground_object_h/2;
                var x=0;var y;
                var xInter=10;
                lineStyle(1,0xff8000);
                while(true){
                    x+=xInter;
                    if((w-xInter)<x)break;
                    y=yMin_rand+random(rand_sum);
                    lineTo(x,y);
                    lineTo(x-random(xInter),0);moveTo(x,y);
                    lineTo(x+random(xInter),0);moveTo(x,y);
                    lineTo(x-random(xInter),ground_object_h);moveTo(x,y);
                    lineTo(x+random(xInter),ground_object_h);moveTo(x,y);
                }
            }
        }
        //
        var mx=new flash.geom.Matrix();
        function draw_mainLand(){
            beginBitmapFill(ep.ground_object,mx,true);
            lineTo(w,0);lineTo(w,ground_object_h);
            lineTo(0,ground_object_h);lineTo(0,0);
            endFill();
        }
        function draw_land(){
            draw_mainLand();
            if(soft_ground==true){
                landTest_function=land_contact;
                draw_unstableLand();
            }
        }
        draw_land();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ground_obj");

    //arc
    int arc_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/9/arc.dbl",arc_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"arc");

    //launcher
    int launcher_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/9/launcher.dbl",launcher_sz);swf_sprite_placeobject(presprite,bmp,0);
    actionf_sprite(presprite,buf,"var launcher_h=%u;var arc_h=%u",launcher_sz[1],arc_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];var j=ep.jumper;
        function landTest_function(){
            attachMovie('arc','arc',getNextHighestDepth());arc._y=launcher_h;
            setTimeout(reload_launcher,1000);
            _y-=arc_h;
            j.zone_ground._y+=arc_h;
            var factor=7/4;
            j.y_step=0-(j.jump_up*factor);
            j.x_step*=factor;
            j.onEnterFrame=j.jumping;
        }
        function reload_launcher(){
            arc.removeMovieClip();
            _y+=arc_h;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"launcher_obj");

    //ship

    int jumper_main_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/9/jumper.dbl",jumper_main_sz);
    swf_exports_add(dbl,"jumper_main");

    int glass_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/9/glass.dbl",glass_sz);
    swf_exports_add(dbl,"glass");

    int circar_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/9/circar.dbl",circar_sz);
    swf_sprite_placeobject_coords(presprite,bmp,0,0-(circar_sz[0]/2),0-circar_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        function to_vertical(){
            onEnterFrame=function(){
                if(Math.abs(_rotation)<step){_rotation=0;onEnterFrame=undefined;}
                else move_driver();
            }
        }
        var sense=1;var step=ep.jumper.rot_step;
        function move(){
            onEnterFrame=to_vertical;
            move_driver();
        }
        function move_driver(){
            _rotation+=step*sense;
            if(_rotation>30)sense*=-1;
            else if(_rotation<-30)sense*=-1;
        }
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"driver");

    //se calculeaza cat sa fie distantele pana la urmatoarea
    double jump_decrement_max=double(24)/fps;
    int jump_up=20;int jump_all=jump_up+jumper_main_sz[1];
    double y_step=0-jump_up;double inter_h=0;
    //
    int x_step_value=2*jumper_main_sz[0]/fps;
    int inter_w=0;
    //
    while(y_step<0){
        inter_h-=y_step;
        double st=jump_decrement_max*(jumper_main_sz[1]-y_step)/jump_all;
        y_step+=st;
        inter_w+=x_step_value;
    }
    inter_h-=jumper_main_sz[1]/2;

    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u",width);
    actionf_sprite(presprite,buf,"var rot_step=%u;var x_step_value=%u",90/fps,x_step_value);
    actionf_sprite(presprite,buf,"var jump_decrement_max=%lf;var jump_up=%u;var jump_all=%u",jump_decrement_max,jump_up,jump_all);
    actionf_sprite(presprite,buf,"var jumper_main_w=%u;var jumper_main_h=%u;var glass_w=%u;var glass_h=%u;var driver_w=%u;var driver_h=%u",jumper_main_sz[0],jumper_main_sz[1],glass_sz[0],glass_sz[1],circar_sz[0],circar_sz[1]);
    action_sprite(presprite,R"(
        var ep=_root['episode'];
        var com=_root['shared_level'];
        var x_step;
        function move(){
            if(Key.isDown(com.jump_aux_key)==true){
                zone_ground._y+=jumper_main_h;attachMovie('arc','arc',getNextHighestDepth());arc._y=jumper_main_h;
                y_step=0-jump_up;
                interactive_x_step();
                mc_stand=undefined;
                onEnterFrame=jumping;
            }
            else if(Key.isDown(com.left_arrow)==true){
                jumper_main.rot=0-rot_step;
                jumper_main.rotate_bmp();
                driver.move();
                l_move(x_step_value);
                fall_test();
            }
            else if(Key.isDown(com.right_arrow)==true){
                jumper_main.rot=rot_step;
                jumper_main.rotate_bmp();
                driver.move();
                r_move(x_step_value);
                fall_test();
            }
        }
        function interactive_x_step(){
            if(Key.isDown(com.left_arrow)==true)x_step=0-x_step_value;
            else if(Key.isDown(com.right_arrow)==true)x_step=x_step_value;
            else x_step=0;
        }
        function l_move(st){_x-=st;if(_x<0)_x=0;}
        function r_move(st){_x+=st;if(width<(_x+jumper_main_w))_x=width-jumper_main_w;}
        onEnterFrame=move;
        //
        function fall_test(){
            if(standing_Test()==true)return null;
            start_falling();
        }
        //x step aici ca poate veni si de la cand se rupe un pod
        function start_falling(){
            y_step=0;
            interactive_x_step();
            mc_stand=undefined;
            onEnterFrame=landing;
        }
        //
        function jumping(){
            jump_move();
            if(y_step>0)go_landing();
            else{
                var clips=zone_ground.clips;
                for(var mc_name in clips){
                    var mc=clips[mc_name];
                    if(hitTest(mc)==true){
                        zone_ground._y+=y_step;
                        y_step=0;
                        go_landing();
                        break;
                    }
                }
            }
        }
        function go_landing(){arc.removeMovieClip();onEnterFrame=landing;}
        //asta e ca sa nu se urce-n sus unde y-ul inainte era mai jos
        var prev_ground_y;var next_ground;
        function landing(){
            prev_ground_y=zone_ground._y;
            jump_move();
            var clips=zone_ground.clips;
            for(var mc_name in clips){
                var mc=clips[mc_name];
                if(land_Test(mc)==true){
                    if(_y+jumper_main_h<(mc._y+prev_ground_y)){
                        mc_stand=mc;
                        zone_ground._y+=_y-((mc._y+zone_ground._y)-jumper_main_h);
                        if(mc.ender==undefined){onEnterFrame=move;mc.landTest_function();}
                        else{
                            var s=mc.end_sense;
                            var next_ground=ep.add_zone();
                            ep['next_ground']=next_ground;
                            next_ground._x=width*s;next_ground._y=0-(ep.land_y()-jumper_main_h-_y);
                            onEnterFrame=function(){
                                jumper_main.rot=rot_step*s;jumper_main.rotate_bmp();driver.move();
                                var v=x_step_value*s;
                                zone_ground._x-=v;next_ground._x-=v;
                                if(hitTest(mc)==false){
                                    onEnterFrame=function(){
                                        var a=next_ground._x<0;
                                        var v=x_step_value*s*2;
                                        zone_ground._x-=v;next_ground._x-=v;_x-=v;
                                        if(a!=(next_ground._x<0)){
                                            _x-=next_ground._x;
                                            next_ground._x=0;
                                            zone_ground.removeMovieClip();
                                            zone_ground=next_ground;
                                            onEnterFrame=move;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
        function jump_move(){
            var st=jump_decrement_max*(jumper_main_h-y_step)/jump_all;
            y_step+=st;
            zone_ground._y-=y_step;
            if(x_step<0)l_move(0-x_step);else if(x_step>0)r_move(x_step);
        }
        var mc_stand;
        //bool
        function land_Test(mc){
            if(hitTest(mc)==false)return false;
            if(x_land_Test(mc)==false)return false;
            return true;
        }
        //bool
        function standing_Test(){
            return x_land_Test(mc_stand);
        }
        //bool
        function x_land_Test(mc){
            var b=driver.getBounds(_root);var mc_b=mc.getBounds(_root);
            if(b.xMin<mc_b.xMin)if(b.xMax<mc_b.xMin)return false;
            if(mc_b.xMax<b.xMin)return false;
            return true;
        }
        //
        attachMovie('driver','driver',getNextHighestDepth());
        //
        createEmptyMovieClip('jumper_main',getNextHighestDepth());
        var mx=new flash.geom.Matrix();
        //
        var mc=jumper_main.createEmptyMovieClip('glass',jumper_main.getNextHighestDepth());
        var glass_bmp=flash.display.BitmapData.loadBitmap('glass');
        mc.beginBitmapFill(glass_bmp,mx,false);
        mc.lineTo(glass_w,0);mc.lineTo(glass_w,glass_h);mc.lineTo(0,glass_h);mc.lineTo(0,0);
        mc.endFill();
        var glass_y=(jumper_main_h-glass_h)/2;
        mc._alpha=50;mc._x=(jumper_main_w-glass_w)/2;mc._y=glass_y;
        //
        var mc=jumper_main.createEmptyMovieClip('jumper_main',jumper_main.getNextHighestDepth());
        var jumper_main_bmp=flash.display.BitmapData.loadBitmap('jumper_main');
        mc.beginBitmapFill(jumper_main_bmp,mx,false);
        mc.lineTo(jumper_main_w,0);mc.lineTo(jumper_main_w,jumper_main_h);mc.lineTo(0,jumper_main_h);mc.lineTo(0,0);
        mc.endFill();
        //
        var jumper_round=new flash.display.BitmapData(jumper_main_w,jumper_main_h,true,0);
        jumper_round.draw(jumper_main);jumper_main.removeMovieClip();
        //
        driver._x=(jumper_main_w-driver_w)/2+(driver_w/2);driver._y=glass_y+glass_h;
        com.mc_rotation_bmp_show(jumper_round,this,'jumper_main',getNextHighestDepth(),rot_step);
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"jumper");

    //bg
    dbl=swf_dbl("C:/Desktop/universe/img/9/bg.dbl");
    swf_exports_add(dbl,"bg_texture");
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u",width);
    action_sprite(presprite,R"(
        var bg_bmp=flash.display.BitmapData.loadBitmap('bg_texture');
        var mx=new flash.geom.Matrix();
        beginBitmapFill(bg_bmp,mx,true);
        lineTo(width,0);lineTo(width,h);
        lineTo(0,h);lineTo(0,0);
        endFill();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bg");

    //rail
    int rail_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/9/rail.dbl",rail_sz);
    swf_exports_add(dbl,"rail_texture");
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var rail_h=%u",width,rail_sz[1]);
    action_sprite(presprite,R"(
        var rail_bmp=flash.display.BitmapData.loadBitmap('rail_texture');
        var mx=new flash.geom.Matrix();
        beginBitmapFill(rail_bmp,mx,true);
        lineTo(width,0);lineTo(width,rail_h);
        lineTo(0,rail_h);lineTo(0,0);
        endFill();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"rail");

    //zones
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var width=%u;var height=%u;var fps=%u",width,height,fps);
    //
    actionf_sprite(presprite,buf,"var ground_object_w=%u;var inter_h=%u;var inter_w=%u",ground_sz[0],int(inter_h),inter_w);
    //
    actionf_sprite(presprite,buf,"var jumper_main_w=%u",jumper_main_sz[0]);//var driver_w=%u;,circar_sz[0]
    action_sprite(presprite,R"(
        var ep=_root['episode'];var soft_ground;var y;
        function new_zone(){
            attachMovie('bg','bg',getNextHighestDepth());bg.swapDepths(clips);
            //pun aicea ca sa mai pun la lrJump
            bg['h']=0;
            if(ep.zoneNr==0)zone_basic();
            else if(ep.zoneNr==1){soft_ground=true;zone_basic();}
            else if(ep.zoneNr==2)zone_lrMoving(undefined);
            else if(ep.zoneNr==3)zone_lrMoving(true);
            else if(ep.zoneNr==4)zone_launcher();
            else if(ep.zoneNr==5){soft_ground=true;zone_launcher();}
            else if(ep.zoneNr==6)zone_upJump(undefined);
            else if(ep.zoneNr==7)zone_upJump(true);
            else if(ep.zoneNr==8)zone_lrJump();
            else if(ep.zoneNr==9){soft_ground=true;zone_lrJump();}
            else _root.end_scenario();
            //
            bg._y=y-height;bg.h+=Math.abs(bg._y)+height;
            //
            ep.zoneNr++;
        }
        //mc
        function one_ground_obj(){
            var d=clips.getNextHighestDepth();
            var mc=clips.attachMovie('ground_obj','land'+d,d);
            mc['w']=ground_object_w;
            return mc;
        }
        function set_end_mc(mc,s){
            var _0_1=(s+1)/2;var _1_0=(s&2)/2;
            var x1=_0_1*width;
            var x2=((mc._x+mc.w)*_1_0)|(mc._x*_0_1);
            mc.w=Math.abs(x2-x1);mc._x=Math.min(x1,x2);
            mc['ender']=true;mc['end_sense']=s;
        }
        function moving_ground_init(){
            createEmptyMovieClip('moving_ground_rail',getNextHighestDepth());moving_ground_rail.swapDepths(clips);
        }
        function moving_ground_add_rail(){
            var d=moving_ground_rail.getNextHighestDepth();
            var mc=moving_ground_rail.attachMovie('rail','rail'+d,d);
            mc._y=y;
        }
    )");
    //
    action_sprite(presprite,R"(
        function zone_basic(){
            y=clips.land._y-inter_h;
            var right_l=width-ground_object_w;
            var x=random(right_l);
            var s=1;
            var inter_width=inter_w+ground_object_w;
            var items_nr=10;
            //
            while(true){
                var mc=one_ground_obj();
                mc._x=x;mc._y=y;mc['soft_ground']=soft_ground;
                items_nr--;
                if(items_nr==0)break;
                var new_x=inter_width*s+x;
                if(right_l<new_x)s*=-1;
                else if(new_x<0)s*=-1;
                x+=inter_width*s;y-=inter_h;
            }
            mc.soft_ground=undefined;
            set_end_mc(mc,s);
        }
    )");
    //
    //aicea buguri cu widthul si gcc
    int wd=width;
    int nr_onW=wd/((2*ground_sz[0])+inter_w-jumper_main_sz[0]);
    double one_on_w=wd/nr_onW;
    double off_to_one=(one_on_w-ground_sz[0])/2;
    actionf_sprite(presprite,buf,"var nr_onW=%u;var one_on_w=%lf;var off_to_one=%lf",nr_onW,one_on_w,off_to_one);
    action_sprite(presprite,R"(
        function zone_lrMoving(soft_bool_1){
            moving_ground_init();
            y=clips.land._y-inter_h;
            var step=int(ground_object_w/(2*fps));
            var fr_1=int(off_to_one/step);var fr_2=int((off_to_one+one_on_w)/step);
            //
            var soft_bool;
            //
            var n=0;while(n<9){
                if((n&1)==0){
                    var x=0;
                    soft_bool=undefined;
                    var sense=1;
                    var at_x_min=fr_2;var at_x_max=fr_1;var frames_go=at_x_min;var to_center=false;
                }
                else{
                    var x=one_on_w;
                    soft_bool=soft_bool_1;
                    var sense=-1;
                    var at_x_min=fr_1;var at_x_max=fr_2;var frames_go=at_x_max;var to_center=false;
                }
                var i=0;while(i<(nr_onW-1)){
                    var mc=one_ground_obj();
                    mc._x=x+off_to_one;mc._y=y;
                    mc['min_fr']=at_x_min;mc['max_fr']=at_x_max;
                    mc['frames_go']=frames_go;mc['to_center']=to_center;
                    mc['sense']=sense;
                    //
                    mc.onEnterFrame=function(){
                        this._x+=step*this.sense;
                        if(ep.jumper.mc_stand==this)ep.jumper._x+=step*this.sense;
                        this.frames_go--;
                        if(this.frames_go==0){
                            if(this.to_center==false){
                                if(this.sense==1)this.frames_go=this.max_fr;else this.frames_go=this.min_fr;
                                this.to_center=true;
                                this.sense*=-1;
                            }
                            else{
                                if(this.sense==1)this.frames_go=this.max_fr;else this.frames_go=this.min_fr;
                                this.to_center=false;
                            }
                        }
                    }
                    mc['soft_ground']=soft_bool;
                    //
                    i++;x+=one_on_w;
                }
                moving_ground_add_rail();
                y-=inter_h;n++;
            }
            //
            mc=one_ground_obj();
            if(random(2)==0){sense=-1;mc._x=width/2-ground_object_w;}
            else{sense=1;mc._x=width/2;}
            mc._y=y;
            set_end_mc(mc,sense);
        }
    )");
    action_sprite(presprite,R"(
        function zone_launcher(){
            y=clips.land._y-inter_h;
            var x=width/4;
            var s=1;
            var items_nr=5;
            //
            while(true){
                var d=clips.getNextHighestDepth();
                var mc=clips.attachMovie('launcher_obj','launcher'+d,d);
                mc._x=x;mc._y=y;
                x+=width/2*s;y-=2*inter_h;
                //
                d=clips.getNextHighestDepth();
                mc=clips.attachMovie('ground_obj','land'+d,d);
                var g_x=x-(ground_object_w*s);
                if(s==1)mc['w']=width-g_x;
                else{mc['w']=g_x;g_x=0;}
                mc._x=g_x;mc._y=y;
                if(soft_ground!=undefined){
                    if(items_nr!=1){
                        mc['soft_ground']=soft_ground;
                        mc['soft_extra_time']=2000;
                    }
                }
                //
                items_nr--;
                if(items_nr==0)break;
                y-=inter_h;s*=-1;
            }
            set_end_mc(mc,s);
        }
    )");
    action_sprite(presprite,R"(
        function zone_upJump(soft_bool_0){
            y=clips.land._y-inter_h;
            //sens mai intai 0 1 pe urma devine -1 1
            var sense=random(2);
            var x=width/2+(inter_w/2);
            x-=(jumper_main_w+inter_w)*sense;
            sense=(sense*2)-1;
            //
            var i=0;while(i<10){
                var d=clips.getNextHighestDepth();
                var mc=clips.attachMovie('ground_obj','land'+d,d);
                mc._x=x;mc._y=y;mc['w']=jumper_main_w;
                if((i&1)==0)mc['soft_ground']=soft_bool_0;
                else mc['soft_ground']=true;
                x+=(jumper_main_w+inter_w)*sense;sense*=-1;
                i++;y-=inter_h;
            }
            mc.soft_ground=undefined;
            set_end_mc(mc,sense);
        }
    )");
    actionf_sprite(presprite,buf,R"(
        function groundEntrance(y_pos,st,from,to){
            this.y=y_pos;
            this.step=st;
            this.x=from;
            this.x_last=to;
        }
        function zone_lrJump(){
            moving_ground_init();
            y=clips.land._y-inter_h;
            var _0_1__1_0=random(2);
            var sense=(_0_1__1_0*2)-1;
//aicea trebuie cel putin jumate de secunda
            var stable_sz=%u;
            var entrances=new Array();
            var step=ground_object_w/fps;var x1=0-ground_object_w;var x2=width;var from;var to;
            var wait_frames=((7/2*ground_object_w)+inter_w)/step;
            var i=0;while(true){
                var d=clips.getNextHighestDepth();
                var mc=clips.attachMovie('ground_obj','land'+d,d);
                mc._x=(width*_0_1__1_0)-(stable_sz*_0_1__1_0);mc._y=y;mc['w']=stable_sz;
                if(i==2)break;
                y-=inter_h;
                if(sense==-1){from=x2;to=x1;}else{from=x1;to=x2;}
                var e=new groundEntrance(y,step*sense,from,to);
                entrances.push(e);
                sense*=-1;_0_1__1_0=(_0_1__1_0-1)/-1;
                i++;
                moving_ground_add_rail();
                y-=inter_h;
            }
            set_end_mc(mc,sense);
            //
            var dst=height/3;
            var lrJump_view_step=dst/fps;
            var lrJump_dst_y=height-dst;
            bg.h+=dst;
            onEnterFrame=function(){
                lrJump_move_rows();
                var jp=ep.jumper;
                jp._y-=lrJump_view_step;
                jp.zone_ground._y-=lrJump_view_step;this._y-=lrJump_view_step;
                if(jp._y<lrJump_dst_y){
                    //sa fie fara probleme la background
                    var yVal=lrJump_dst_y-jp._y;
                    jp.zone_ground._y+=yVal;this._y+=yVal;
                    jp._y=lrJump_dst_y;
                    //
                    onEnterFrame=lrJump_move_rows;
                }
            }
            //
            var frame=wait_frames;
            function lrJump_move_rows(){
                frame++;if(frame<wait_frames)return null;
                frame=0;
                //
                var i=0;while(i<entrances.length){
                    var mc=one_ground_obj();
                    mc._x=entrances[i].x;mc._y=entrances[i].y;mc['soft_ground']=soft_ground;
                    mc['step']=entrances[i].step;mc['x_last']=entrances[i].x_last;
                    mc.onEnterFrame=function(){
                        var a=this._x<this.x_last;
                        this._x+=this.step;
                        if(a!=(this._x<this.x_last))this.removeMovieClip();
                        else if(ep.jumper.mc_stand==this){
                            ep.jumper._x+=this.step;
                            var a=ep.jumper._x<0;
                            if(a==true)ep.jumper._x=0;
                            else{
                                var a=(width-jumper_main_w)<ep.jumper._x;
                                if(a==true)ep.jumper._x=width-jumper_main_w;
                            }
                            if(a==true)ep.jumper.fall_test();
                        }
                    }
                    i++;
                }
            }
        }
    )", x_step_value*(fps / 2));
    action_sprite(presprite,R"(
        new_zone();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ground");

    //

    actionf(buf,"var width=%u;var height=%u",width,height);
    actionf(buf,"var ground_h=%u",ground_sz[1]);
    actionf(buf,"var jumper_main_w=%u;var jumper_main_h=%u",jumper_main_sz[0],jumper_main_sz[1]);
    action(R"(
        var zoneNr=0;
        createEmptyMovieClip('ground_zone',getNextHighestDepth());
        //ground
        function add_zone(){
            var d=ground_zone.getNextHighestDepth();
            var mv=ground_zone.attachMovie('ground','ground'+d,d);
            var mc=mv.createEmptyMovieClip('clips',mv.getNextHighestDepth());
            var land=mc.attachMovie('ground_obj','land',mc.getNextHighestDepth());
            land['w']=width;
            land._y=land_y();
            return mv;
        }
        function land_y(){
            return height-ground_h-jumper_main_h-10;
        }
        var mc=add_zone();
        attachMovie('jumper','jumper',getNextHighestDepth());
        jumper['zone_ground']=mc;
        jumper._y=mc.clips.land._y-jumper_main_h;
        jumper._x=(width-jumper_main_w)/2;
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
