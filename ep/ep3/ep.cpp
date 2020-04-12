
#include<windows.h>
#include<stdio.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

//(1048575) and lowest (-16383)  si pun tot timpul obiecte cu getNextHighestDepth dar nu e periculos

#ifdef __cplusplus
extern "C" {
#endif

int _start(){
    char* ep_nm=Planet_Landing_ep;
	sprintf(buf, "%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);
    action("_root['episode']=this");
    init_episod(ep_nm);

    int presprite;int sprite;int bmp;

    //ship_fuel_on
    int fuel_sz[2];
    presprite=swf_sprite_new();
    //
    bmp=swf_image("C:/Desktop/universe/img/3/fuel.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    //
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_fuel");

    presprite=swf_sprite_new();
    //
    bmp=swf_image_ex("C:/Desktop/universe/img/3/fuel1.dbl",fuel_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/3/fuel2.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/3/fuel3.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_fuel_anim");


    //ship
    int ship_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/3/ship.dbl",ship_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_body");
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var ship_fuel_h=%u",fuel_sz[1]);
    action_sprite(presprite,R"(
        attachMovie('ship_body','ship_body',0);
        var mv_name='ship_fuel_L';attachMovie('ship_fuel',mv_name,1);var mv=this[mv_name];
        mv._x=4;mv._y=17;mv._rotation=180;mv._y+=ship_fuel_h;
        mv_name='ship_fuel_Dl';attachMovie('ship_fuel',mv_name,2);mv=this[mv_name];
        mv._x=8;mv._y=33;mv._rotation=90;mv._x+=ship_fuel_h;
        mv_name='ship_fuel_Dr';attachMovie('ship_fuel',mv_name,3);mv=this[mv_name];
        mv._x=24;mv._y=33;mv._rotation=90;mv._x+=ship_fuel_h;
        mv_name='ship_fuel_R';attachMovie('ship_fuel',mv_name,4);mv=this[mv_name];
        mv._x=36;mv._y=17;
        _root['episode'].start_fuel_D();
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship");

    //boom
    presprite=swf_sprite_new();
    bmp=swf_image("C:/Desktop/universe/img/3/boom1.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/2;i++)swf_sprite_showframe(presprite);
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/3/boom2.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/2;i++)swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"boom");

    //

    float step=ship_sz[0];float part=fps;part/=2;step/=part;
    actionf(buf,"var step=%u;var width=%u;var height=%u;var ship_w=%u;var ship_h=%u",int(step),width,height,ship_sz[0],ship_sz[1]);

	action(R"(
    var inter_rows=ship_h/2;
    var row_h=ship_h+inter_rows;

    function add_fuel(ship_part){
        if(ship[ship_part].ship_fuel_anim!=undefined)return undefined;
        ship[ship_part].attachMovie('ship_fuel_anim','ship_fuel_anim',ship[ship_part].getNextHighestDepth());
        ship[ship_part]['ship_fuel_anim']._alpha=90;
    }
    function start_fuel_D(){
        add_fuel('ship_fuel_Dl');
        add_fuel('ship_fuel_Dr');
    }
    function stop_fuel_D(){
        ship.ship_fuel_Dl.ship_fuel_anim.removeMovieClip();
        ship.ship_fuel_Dr.ship_fuel_anim.removeMovieClip();
    }
    function stop_fuel_L(){
        ship.ship_fuel_L.ship_fuel_anim.removeMovieClip();
    }
    function stop_fuel_R(){
        ship.ship_fuel_R.ship_fuel_anim.removeMovieClip();
    }
    function active_ship(){
        ship_testHit();

        if(Key.isDown(com.left_arrow)==true){
            add_fuel('ship_fuel_R');
            var left_right_step=step;

            if(ship['ship_test_hit']==true)left_right_step/=2;
            //return null;

            ship._x-=left_right_step;
            if(ship._x<0)ship._x=0;
        }
        else if(Key.isDown(com.right_arrow)==true){
            add_fuel('ship_fuel_L');
            var left_right_step=step;

            if(ship['ship_test_hit']==true)left_right_step/=2;
            //return null;

            ship._x+=left_right_step;
            if(width-ship_w<ship._x)ship._x=width-ship_w;
        }
        if(Key.isDown(com.down_arrow)==true){
            stop_fuel_D();

            if(ship['ship_test_hit']==true)return null;

            zone._y-=step;
            //ok = cu stepul
            if(zone._y<=(0-zone_end)){
                stop_fuel_L();stop_fuel_R();stop_fuel_D();Key.removeListener(ship);

                prev_zone=zone;
                add_next_zone();
            }
        }
    }
    function ship_testHit(){
		var a=(zone._y+start_y)*-1/row_h;
        var row_nr=Math.floor(a);
		//int pentru primul rand nu da bine ca e -0.x si da 0, trebuie -1
        if(ship_testHit_row(zone['row'+row_nr])==true){}
        else if(ship_testHit_row(zone['row'+(row_nr+1)])==true){}
        else{
            if(ship['ship_test_hit']==true){
                ship['boom'].removeMovieClip();
                ship['ship_test_hit']=false;
            }
            return null;
        }
        if(ship['ship_test_hit']!=true){
            if(ship.boom!=undefined)ship.boom.removeMovieClip();
            ship.attachMovie('boom','boom',ship.getNextHighestDepth());ship['boom']._alpha=50;
            ship['ship_test_hit']=true;
            zone._y=row_h*row_nr*-1-start_y;
        }
    }
    //bool
    function ship_testHit_row(row){
        var i=row.getNextHighestDepth();i--;var mv=row.getInstanceAtDepth(i);var mc=ship.ship_body;
        while(mv!=undefined){
            if(mc.hitTest(mv)==true)return true;
            i--;var mv=row.getInstanceAtDepth(i);
        }
        return false;
    }
    )");

    action(R"(
    var cloud_w=ship_w*2;var cloud_h=ship_h;
    var cloud_part=20;var cloud_indent=10;
    //mc
    function draw_cloud(container,depth,color){
        var mv=container.createEmptyMovieClip('cloud'+depth,depth);

        mv.beginFill(color,75);

        var start_x=random(cloud_part);

        mv.moveTo(start_x,cloud_indent);
        var next_x=cloud_part+random(cloud_part);
        mv.curveTo(cloud_part,0,next_x,cloud_indent);
        next_x=2*cloud_part+random(cloud_part);
        mv.curveTo(2*cloud_part,0,next_x,cloud_indent);
        next_x=3*cloud_part+random(cloud_part);
        mv.curveTo(3*cloud_part,0,next_x,cloud_indent);

        var store_y=cloud_h-cloud_indent;

        next_x=cloud_w-random(cloud_part);
        mv.curveTo(cloud_w,cloud_h/2,next_x,store_y);

        next_x=3*cloud_part-random(cloud_part);
        mv.curveTo(3*cloud_part,cloud_h,next_x,store_y);
        next_x=2*cloud_part-random(cloud_part);
        mv.curveTo(2*cloud_part,cloud_h,next_x,store_y);
        next_x=cloud_part-random(cloud_part);
        mv.curveTo(cloud_part,cloud_h,next_x,store_y);

        mv.curveTo(0,cloud_h/2,start_x,cloud_indent);

        mv.endFill();

        return mv;
    }
    )");

    //ozn
    int ozn_sz[2];
    for(int z=0;z<2;z++){
        presprite=swf_sprite_new();
        sprintf(buf,"C:/Desktop/universe/img/3/ozn%u.dbl",z);
        bmp=swf_image_ex(buf,ozn_sz);
        swf_sprite_placeobject(presprite,bmp,0);
        action_sprite(presprite,"var ep=_root['episode'];ep.draw_ozn_spin(this)");
        for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
        action_sprite(presprite,"ep.draw_ozn_spin(this)");
        for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
        action_sprite(presprite,"ep.draw_ozn_spin(this)");
        for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
        sprite=swf_sprite_done(presprite);
        sprintf(buf,"ozn%u",z);swf_exports_add(sprite,buf);
    }
    #define ozn_spin ozn_sz[0]/2
    actionf(buf,"var ozn_w=%u;var ozn_h=%u;var ozn_spin=%u",ozn_sz[0],ozn_sz[1],ozn_spin);
    actionf(buf,"var ozn_spin_speed=%u",ozn_spin/(fps/2));
    action(R"(
        function draw_ozn_spin(ozn){
            if(ozn.step>0){
                if(ozn.ozn_nr<ozn_spin)ozn.ozn_nr+=ozn_spin_speed;
                else ozn.ozn_nr=0;
            }
            else{
                if(ozn.ozn_nr>0)ozn.ozn_nr-=ozn_spin_speed;
                else ozn.ozn_nr=ozn_spin;
            }
            var x=ozn.ozn_nr;var c=ozn_w/2;

            var ozn_mc=ozn.spin;
            ozn_mc.clear();
            //if(ozn_mc.color==0xff0000)ozn_mc.color=0x00ff00;
            //else ozn_mc.color=0xff0000;
            ozn_mc.lineStyle(0,0x202020,50);
            while(x<ozn_w){
                ozn_mc.moveTo(x,ozn_h);ozn_mc.lineTo(c,0);
                x+=ozn_spin;
            }
        }
        //mc
        function ozn_init(row,d){
            var mc_name='ozn'+d;
            var mc=row.attachMovie('ozn'+random(2),mc_name,d);
            mc.createEmptyMovieClip('spin',mc.getNextHighestDepth());
            return mc;
        }
    )");

    //lighting_init
    action(R"(
        var lighting_w=width;var lighting_h=ship_h;
        function draw_lighting(mc,parts,min_x,max_x,min_y,max_y,x_axis,current_x,current_y,yAxisEnd){
            var x;var y;
            //ok cu = parts
            var i=1;while(i<=parts){
                if(x_axis==true){
                    x=careful_random(max_x-min_x)+min_x;
                    y=current_y-((current_y-max_y)*i/parts);
                }
                else{
                    x=current_x-((current_x-max_x)*i/parts);
                    y=careful_random(max_y-min_y)+min_y;
                }
                mc.lineTo(x,y);
                if(i<parts){
                    if(yAxisEnd==true){
                        draw_lighting(mc,parts-1,x,x+((max_x-x)/2),min_y,min_y,true,x,y,yAxisEnd);
                        mc.moveTo(x,y);
                        draw_lighting(mc,parts-1,x,x+((max_x-x)/2),max_y,max_y,true,x,y,yAxisEnd);
                        mc.moveTo(x,y);
                        min_x=x;
                    }else{
                        draw_lighting(mc,parts-1,min_x,min_x,y,y+((max_y-y)/2),true,x,y,yAxisEnd);
                        mc.moveTo(x,y);
                        draw_lighting(mc,parts-1,max_x,max_x,y,y+((max_y-y)/2),true,x,y,yAxisEnd);
                        mc.moveTo(x,y);
                        min_y=y;
                    }
                }
                i++;
            }
        }
        function careful_random(n){
            if(n==0)return 0;
            else return random(n);
        }
        //mc
        function lighting_init(row,d){
            var mc_name='lighting'+d;
            var mc=row.createEmptyMovieClip(mc_name,d);
            mc.lineStyle(2,0xffFFff);
            mc.moveTo(0,random(lighting_h));
            draw_lighting(mc,3,lighting_w,lighting_w,0,lighting_h,false,0,null,true);
            glow_light(mc);
            return mc;
        }
        function glow_light(lighting){
            var filter=new flash.filters.GlowFilter(0xffFFff,8/10,128,128,16);
            var filterArray=new Array(filter);
            lighting.filters=filterArray;
        }
    )");

    //rock
    int rock_sz[2];
    for(int z=0;z<2;z++){
        sprintf(buf,"C:/Desktop/universe/img/3/rock%u.dbl",z);
        dbl=swf_dbl_ex(buf,rock_sz);
        sprintf(buf,"rock%u",z);swf_exports_add(dbl,buf);
    }
    actionf(buf,"var rock_w=%u;var rock_h=%u",rock_sz[0],rock_sz[1]);
    action(R"(
        var BitmapData=flash.display.BitmapData;
        var rock_0_pre=BitmapData.loadBitmap('rock0');
        var rock_1_pre=BitmapData.loadBitmap('rock1');
        //pun margini transparente ca altfel: When repeat is set to false, the bitmap fill uses the edge pixels for the fill area outside of the bitmap
        var fix_margins=2;
        var rock_bmp_0=new BitmapData(rock_0_pre.width+fix_margins,rock_0_pre.height+fix_margins,true,0);
        var rock_bmp_1=new BitmapData(rock_1_pre.width+fix_margins,rock_1_pre.height+fix_margins,true,0);
        var sourceRect=new flash.geom.Rectangle(0,0,rock_0_pre.width,rock_0_pre.height);
        var destPoint=new flash.geom.Point(1,1);
        rock_bmp_0.copyPixels(rock_0_pre,sourceRect,destPoint);
        rock_bmp_1.copyPixels(rock_1_pre,sourceRect,destPoint);
        delete rock_0_pre;
        delete rock_1_pre;
        //mc
        function rock_init(row,d){
            var mc=row.createEmptyMovieClip('rock'+d,d);
            //
            var mv=mc.createEmptyMovieClip('rock',0);
            var bmp=_root['episode']['rock_bmp_'+random(2)];
            var mx=new flash.geom.Matrix();
            mx.translate(0-(bmp.width/2),0-(bmp.height/2));
            mv['mx']=mx;mv['bmp']=bmp;mv._x=rock_w/2;mv._y=rock_h/2;
            if(row.object_startX<0)mv['rot']=90/fps;
            else mv['rot']=-90/fps;
            mv.onEnterFrame=function(){
                this.mx.rotate(this.rot*Math.PI/180);
                var w=this._x;var h=this._y;
                this.clear();
                this.beginBitmapFill(this.bmp,this.mx,false);
                this.moveTo(0-w,0-h);
                this.lineTo(w,0-h);this.lineTo(w,h);
                this.lineTo(0-w,h);this.lineTo(0-w,0-h);
                this.endFill();
            }
            //
            return mc;
        }
    )");

    //rocket
    int rocket_w;int rocket_body_sz[2];int rocket_fuel_sz[2];
    for(int z=0;z<2;z++){
        presprite=swf_sprite_new();
        sprintf(buf,"C:/Desktop/universe/img/3/rocket%u.dbl",z);
        bmp=swf_image_ex(buf,rocket_body_sz);swf_sprite_placeobject(presprite,bmp,0);
        swf_sprite_showframe(presprite);
        sprite=swf_sprite_done(presprite);
        sprintf(buf,"rocket%u",z);swf_exports_add(sprite,buf);
    }
    //
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/3/rocket_fuel1.dbl",rocket_fuel_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/3/rocket_fuel2.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/3/rocket_fuel3.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"rocket_fuel");
    rocket_w=rocket_body_sz[0]+rocket_fuel_sz[0];
    actionf(buf,"var rocket_w=%u;var rocket_body_w=%u;var rocket_fuel_w=%u",rocket_w,rocket_body_sz[0],rocket_fuel_sz[0]);
    action(R"(
    //mc
    function rocket_init(row,d){
        var mc_name='rocket'+d;
        var mc=row.createEmptyMovieClip(mc_name,d);

        mc.attachMovie('rocket'+random(2),'rocket',0);
        mc.attachMovie('rocket_fuel','rocket_fuel',1);

        if(row.object_startX<0){
            mc['rocket']._x=rocket_fuel_w;
        }
        else{
            mc['rocket']._xscale=-100;mc['rocket']._x=rocket_body_w;
            mc['rocket_fuel']._xscale=-100;mc['rocket_fuel']._x=rocket_fuel_w+rocket_body_w;
        }
        mc['rocket_fuel']._alpha=90;

        return mc;
    }
    )");

    //red_cloud
    action(R"(
        //mc
        function red_cloud_init(row,d){
            var mc=draw_cloud(row,d,0xff0000);
            return mc;
        }
    )");

    //satellite
    for(int z=0;z<2;z++){
        presprite=swf_sprite_new();
        sprintf(buf,"C:/Desktop/universe/img/3/satellite%u.dbl",z);
        bmp=swf_image(buf);swf_sprite_placeobject(presprite,bmp,0);
        swf_sprite_showframe(presprite);
        sprite=swf_sprite_done(presprite);
        sprintf(buf,"satellite%u",z);swf_exports_add(sprite,buf);
    }
    //
    action(R"(
    //mc
    function satellite_init(row,d){
        var mc_name='satellite'+d;
        var mc=row.attachMovie('satellite'+random(2),mc_name,d);
        return mc;
    }
    )");

    //green_cloud_init
    action(R"(
        //mc
        function green_cloud_init(row,d){
            var mc=draw_cloud(row,d,0x00FF00);
            return mc;
        }
    )");

    //small_rocks
    int small_rock_sz[2];
    //trail
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/3/small_rock_trail1.dbl",small_rock_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/2;i++)swf_sprite_showframe(presprite);
    //
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/3/small_rock_trail2.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/2;i++)swf_sprite_showframe(presprite);
    //
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"small_rock_trail");
    //rock and trail
    presprite=swf_sprite_new();
    bmp=swf_image("C:/Desktop/universe/img/3/small_rock.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    //
    action_sprite(presprite,"attachMovie('small_rock_trail','small_rock_trail',1);small_rock_trail._alpha=75");
    swf_sprite_showframe(presprite);
    //
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"rocks");
    //all mc
    presprite=swf_sprite_new();action_sprite(presprite,R"(
        attachMovie('rocks','rocks',0);rocks._xscale=scale_value;rocks._x=set_x_value;
    )");swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"small_rocks");

    actionf(buf,"var small_rock_w=%u",small_rock_sz[0]);
    action(R"(
        var small_rock_number=2;
        var one_part=ship_w/small_rock_number;var x_off;
        //mc
        function small_rocks_init(container_row,d){
            var mc_name='small_rocks'+d;
            var mc=container_row.createEmptyMovieClip(mc_name,d);

            if(container_row.object_startX<0)x_off=one_part-small_rock_w;
            else x_off=0;

            var row=0;
            while(row<small_rock_number){
                var col=0;
                while(col<small_rock_number){
                    var next_d=mc.getNextHighestDepth();
                    var mv=mc.attachMovie('small_rocks','small_rocks'+next_d,next_d);

                    if(container_row.object_startX<0){
                        mv['scale_value']=-100;mv['set_x_value']=small_rock_w;
                    }
                    else{mv['scale_value']=100;mv['set_x_value']=0;}

                    var y=random(one_part);
                    mv._x=col*one_part+x_off;mv._y=row*one_part+y;

                    col++;
                }
                row++;
            }
            return mc;
        }
    )");

    //lighting_cloud_init
    presprite=swf_sprite_new();
    action_sprite(presprite,"clear()");
    for(int i=0;i<fps;i++)swf_sprite_showframe(presprite);
    action_sprite(presprite,"draw_cloud_lighting(this)");
    for(int i=0;i<fps;i++)swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"cloud_lighting");
    //
    action(R"(
        var cloud_lighting_w=ship_w;var cloud_lighting_h=ship_h*3/4;
        //mc
        function lighting_cloud_init(row,d){
            var mc=draw_cloud(row,d,0xc3c3c3);
            //
            var mv=mc.attachMovie('cloud_lighting','cloud_lighting',mc.getNextHighestDepth());
            mv._x=(cloud_w-cloud_lighting_w)/2;mv._y=cloud_h-cloud_lighting_h;
            mv['draw_cloud_lighting']=draw_cloud_lighting;
            glow_light(mv);
            //
            return mc;
        }
        function draw_cloud_lighting(cloud){
            cloud.lineStyle(2,0xffFFff);
            cloud.moveTo(random(cloud_lighting_w),0);
            draw_lighting(cloud,3,0,cloud_lighting_w,cloud_lighting_h,cloud_lighting_h,true,null,0,false);
        }
    )");

    //tornado
    presprite=swf_sprite_new();
    bmp=swf_image("C:/Desktop/universe/img/3/tornado0.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/2;i++)swf_sprite_showframe(presprite);
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/3/tornado1.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/2;i++)swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"tornado");
    //
    action(R"(
        //mc
        function tornado_init(row,d){
            var mc_name='tornado'+d;
            var mc=row.attachMovie('tornado',mc_name,d);
            return mc;
        }
    )");

    actionf(buf,"var fps=%u",fps);
    action(R"(
    //
    var zone_nr=0;var zone_rows=10;
    var start_y=row_h;
	var zone_end=start_y+(zone_rows*row_h);
    //zone
    function add_zone(objects_min,objects_max,objects_interFrames,freeSpace_frames,initial_delay,object_step,object_durationFrames,object_left_startX,object_right_startX,constructor_function){
        var mc_name='zone'+zone_nr;
        var zone=createEmptyMovieClip(mc_name,getNextHighestDepth());
        var mc=this[mc_name];
        var y=start_y;
        while(y<zone_end){
            var next_depth=mc.getNextHighestDepth();var mv_name='row'+next_depth;
            var mv=mc.createEmptyMovieClip(mv_name,next_depth);
            mv._y=y;

            //remove after n frames: fulgerele pleaca de pe ecran, miscatoarele pleaca undeva in offset
            mv['object_durationFrames']=object_durationFrames;

            //numarul de obiecte consecutive
            mv['objects_min']=objects_min;
            mv['objects_max']=objects_max;
            //numarul de frames pe unde trece shipul
            mv['freeSpace_frames']=freeSpace_frames;
            //
            mv['constructor_function']=constructor_function;
            //nr de frames dintre obiecte
            mv['objects_interFrames']=objects_interFrames;

            rowObjectsInit(mv);

            //la impare ca poate sa fie stanga dreapta si e pauza
                    //la froger si din stanga si din dreapta pleaca in acelasi timp la start,conteaza random(min,max)
                    //dar la stanga dreapta nu ar mai fi spatiu deloc asa ca imparele au offset la objects
            if(next_depth&1==0){mv['object_startX']=object_left_startX;mv['object_step']=object_step;}
            else{
                                                                          //fulgerele au 0 iar altele se misca
                mv['object_startX']=object_right_startX;mv['object_step']=0-object_step;
                if(initial_delay!=0){
                    mv['initial_delay']=initial_delay;
                    mv.onEnterFrame=rowInitial;
                }
            }

            y+=row_h;
        }
        return zone;
    }
    function rowInitial(){
        //ok =
        this['initial_delay']--;if(this['initial_delay']<=0)rowObjectsInit(this);
    }

    function rowWaitInit(mv){
        mv['freeSpace_frames_cursor']=mv['freeSpace_frames'];mv.onEnterFrame=rowWait;
    }
    function rowWait(){
        this['freeSpace_frames_cursor']--;
        if(this['freeSpace_frames_cursor']>0)return null;
        rowObjectsInit(this);
    }
    function rowObjectsInit(mv){
        //nu e ok de pus random dintre 0 si 0-1
        if(mv['objects_min']==mv['objects_max'])mv['objects_cursor']=mv['objects_max'];
        else mv['objects_cursor']=random(mv['objects_max']-mv['objects_min'])+mv['objects_min'];
        mv.onEnterFrame=rowObjects;
    }
    function rowObjects(){
        if(this['objects_cursor']<0){rowWaitInit(this);return null;}
        this['objects_cursor']--;

        var constructor_function=this['constructor_function'];
        var mc=constructor_function(this,this.getNextHighestDepth());mc._x=this.object_startX;
        mc['step']=this['object_step'];mc['durationFrames']=this['object_durationFrames'];
        mc.onEnterFrame=objectFrame;

        this['objects_interFrames_cursor']=this['objects_interFrames'];
        this.onEnterFrame=rowInter;
    }
    function rowInter(){
        this['objects_interFrames_cursor']--;
        if(this['objects_interFrames_cursor']>0)return null;
        this.onEnterFrame=rowObjects;
    }

    function objectFrame(){
        //ok =
        this['durationFrames']--;if(this['durationFrames']<=0)this.removeMovieClip();
        this._x+=this['step'];
    }

    //(objects_min,objects_max,objects_interFrames,freeSpace_frames,initial_delay,object_step,object_durationFrames,object_left_startX,object_right_startX,constructor_function)
    var zone;
    function default_twoRows(item_w,item_fn){
        var twoRows_multiStep=3*step;
        var free_space=3*width/twoRows_multiStep;
        var initial_delay=2*width/twoRows_multiStep;
        zone=add_zone(width/item_w,width/item_w,item_w/twoRows_multiStep,free_space,initial_delay,twoRows_multiStep,2*width/twoRows_multiStep,0-item_w,width,item_fn);
    }
    function default_leftRight(item_fn){
        var item_w=ship_w;
        zone=add_zone(1,width/2/item_w,item_w/step,ship_w*4/step,0,step,2*width/step,0-item_w,width,item_fn);
    }
    var zones_last_index=9;
    //bool
    function level_zones(){
        var return_value=false;
        if(zone_nr==0)default_twoRows(ozn_w,ozn_init);
        else if(zone_nr==1){
            zone=add_zone(2,2,fps/2,5*fps,3*fps,0,fps/2,0,0,lighting_init);
            var i=0;
            while(i<zone_rows){
                var mc=zone['row'+i].createEmptyMovieClip('marker',0);
                mc.lineStyle(2,0xffFFff);
                mc.lineTo(0,ship_h);
                var filter=new flash.filters.GlowFilter(0xffFFff,8/10,128,8,16);
                var filterArray=new Array(filter);
                mc.filters=filterArray;
                i++;
            }
        }
        else if(zone_nr==2)default_leftRight(rock_init);
        else if(zone_nr==3)default_twoRows(rocket_w,rocket_init);
        else if(zone_nr==4)default_twoRows(cloud_w,red_cloud_init);
        else if(zone_nr==5)default_leftRight(satellite_init);
        else if(zone_nr==6)default_twoRows(cloud_w,green_cloud_init);
        else if(zone_nr==7)default_leftRight(small_rocks_init);
        else if(zone_nr==8)default_twoRows(cloud_w,lighting_cloud_init);
        else if(zone_nr==zones_last_index)default_leftRight(tornado_init);
        else{
            zone=attachMovie('land','land',getNextHighestDepth());
            ship.swapDepths(zone);
            ship.attachMovie('landers','landers',ship.getNextHighestDepth());
            ship.landers._y=ship_h-landers_h;
            return_value=true;
        }
        zone_nr++;
        return return_value;
    }
    )");

    //start to land

    presprite=swf_sprite_new();
    bmp=swf_image("C:/Desktop/universe/img/3/env.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"env");

    presprite=swf_sprite_new();
    bmp=swf_image("C:/Desktop/universe/img/3/land.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"land");

    int landers_sz[2];
    presprite=swf_sprite_new();
    bmp=swf_image_ex("C:/Desktop/universe/img/3/landers.dbl",landers_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"landers");
    actionf(buf,"var landers_h=%u",landers_sz[1]);

    action(R"(
    attachMovie('env','env',getNextHighestDepth());
    var i=0;
    function intro_for_ship(){
        ship._y=ship_h*i/(2*fps)-ship_h;
        i++;
        if(ship._y==0){
            stop_fuel_D();
            add_next_zone();
        }
    }
    function add_next_zone(){
        var last_zone=level_zones();
        zone._y=height;
        ship.onEnterFrame=ship_to_ready;
    }

    createEmptyMovieClip('atmosphere',getNextHighestDepth());
    attachMovie('ship','ship',getNextHighestDepth());
    ship._x=width/2-ship_w;
    ship._y=0-ship_h;
    ship.onEnterFrame=intro_for_ship;

    var prev_zone;
    function ship_to_ready(){
        prev_zone._y-=step;
        zone._y-=step;
        //ok =
        if(zone._y<=0){
            zone._y=0;
            prev_zone.removeMovieClip();

            if(zone._name!='land'){
                Key.addListener(ship);
                ship.onKeyUp=function(){
                    if(Key.getCode()==com.left_arrow)stop_fuel_R();
                    else if(Key.getCode()==com.right_arrow)stop_fuel_L();
                    else if(Key.getCode()==com.down_arrow)start_fuel_D();
                }
                start_fuel_D();
                ship.onEnterFrame=active_ship;
            }
            else ship.onEnterFrame=landing_phase_1;
        }
        closing_to_land();
    }
    var atmosphere_land_color=0x00ffFF;var all_alpha_levels=1+zones_last_index+1;
    function closing_to_land(){
        var upper_zone_alpha=100*(zone_nr-1)/all_alpha_levels;
        var lower_zone_alpha=100*zone_nr/all_alpha_levels;

        atmosphere.clear();
        atmosphere.beginFill(atmosphere_land_color,upper_zone_alpha);
        atmosphere.lineTo(width,0);atmosphere.lineTo(width,zone._y);
        atmosphere.lineTo(0,zone._y);atmosphere.lineTo(0,0);
        atmosphere.endFill();
        atmosphere.moveTo(0,zone._y);
        atmosphere.beginFill(atmosphere_land_color,lower_zone_alpha);
        atmosphere.lineTo(width,zone._y);atmosphere.lineTo(width,height);
        atmosphere.lineTo(0,height);atmosphere.lineTo(0,zone._y);
        atmosphere.endFill();
    }
    var half_h=height/2;
    function landing_phase_1(){
        ship._y+=step;
        if(ship._y>half_h){ship.onEnterFrame=landing_phase_2;start_fuel_D();}
    }
    var landing_step=ship_h/fps/2;var land_line=half_h+(2*ship_h);
    function landing_phase_2(){
        ship._y+=landing_step;
        if(ship._y>land_line){
            stop_fuel_D();
            ship.onEnterFrame=undefined;
            _root.end_scenario();
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
