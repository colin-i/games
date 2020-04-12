
#include<windows.h>
#include<stdio.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

//sa ajunga la Infinity dar dupa muuuuuuuult timp: fast_number++ la fiecare obiect nou

int _start(){
    char* ep_nm=Driving_the_Ship_ep;
	sprintf(buf, "%s.swf", ep_nm);
    swf_new(buf,width,height,0x01ff01,fps);

    //swf_new("ep.swf",width,height,0x01ff01,fps);

    action("_root['episode']=this");
    init_episod(ep_nm);

    int presprite;int sprite;int bmp;

    //zone

    presprite=swf_sprite_new();bmp=swf_image("C:/Desktop/universe/img/2/zone.dbl");swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"zone");

    //ship

    int ship_front_sz[2];int ship_back_sz[2];
    //
    bmp=swf_image_ex("C:/Desktop/universe/img/2/ship_front.dbl",ship_front_sz);
    presprite=swf_sprite_new();
    swf_sprite_placeobject(presprite,bmp,0);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_front");

    //
    presprite=swf_sprite_new();
    //
    bmp=swf_image_ex("C:/Desktop/universe/img/2/back1.dbl",ship_back_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/2/back2.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/2/back3.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship_back");

    //
    presprite=swf_sprite_new();
    //
    action_sprite(presprite,R"(
        createEmptyMovieClip('shields_counted',0);
        attachMovie('ship_front','front',1);attachMovie('ship_back','back',2);
        _root['episode'].set_mx_y();
    )");swf_sprite_showframe(presprite);
    //
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"ship");

    int ship_w=ship_front_sz[0];
    //main variables
    actionf(buf,"var width=%u;var height=%u;var ship_w=%u",width,height,ship_w);
    float dst=ship_w;
    float part=fps;part/=fps-2;
    dst/=part;
    actionf(buf,"var step=%u;var ship_w=%u",int(dst),ship_w);
    actionf(buf,"var cell=%u;var fps=%u",cell,fps);
    //
    actionf(buf,"var sh_back_half_h=%u;var sh_front_h=%u",ship_back_sz[1]/2,ship_front_sz[1]);
    action(R"(
    var quart_h=height/4;var zone_3_div_4=height-quart_h;
    var half_w_steps=(width-ship_w)/2;
    var half_h_steps=(height-cell)/2;
    var ship_real_h=cell;
    attachMovie('zone','zone',getNextHighestDepth());
    createEmptyMovieClip('celestial_anim',getNextHighestDepth());
    createEmptyMovieClip('speed_anim',getNextHighestDepth());
    attachMovie('ship','ship',getNextHighestDepth());ship._x=half_w_steps;ship._y=zone_3_div_4;

    var max_right=width-ship_w;var max_height=height-cell;
    function move_ship(){
//ca boom_stop!=undefined nu e ok ca la _root.a=mc si mc.removeMovieClip() _root.a devine ""
        if(boom_stop>0)return null;
        if(Key.isDown(com.left_arrow)==true){
            ship._x-=step;
            if(ship._x<0)ship._x=0;
            set_mx_x();
            instant_taker_connections_onShipMove();
        }
        else if(Key.isDown(com.right_arrow)==true){
            ship._x+=step;
            if(ship._x>max_right)ship._x=max_right;
            set_mx_x();
            instant_taker_connections_onShipMove();
        }
        if(Key.isDown(com.up_arrow)==true){
            ship._y-=step;
            if(ship._y<0)ship._y=0;
            set_mx_y();
            instant_taker_connections_onShipMove();
        }
        else if(Key.isDown(com.down_arrow)==true){
            ship._y+=step;
            if(ship._y>max_height)ship._y=max_height;
            set_mx_y();
            instant_taker_connections_onShipMove();
        }
    }
    function set_mx_x(){
        var ship_matrix=ship.transform.matrix;
        var x=ship._x;var coef=1;
        x-=half_w_steps;
        if(x<0){x*=-1;coef=-1;}
        if(ship._y<half_h_steps)coef*=-1;
        var degrees=x*20/half_w_steps*coef;
        var radians=degrees/180*Math.PI;
        ship_matrix.c=Math.tan(radians);
        ship.transform.matrix=ship_matrix;
    }
    var sh_back_half_h;var sh_front_h;
    function set_mx_y(){
        var y=ship._y;var coef=1;
        y-=half_h_steps;
        if(y<0){y*=-1;coef*=-1;}
        //sa mearga la gnash yscale=0(sa nu fie) si sa nu ramana doar spatele
        if(2*cell>y)y=2*cell;
        var y_sc=100*y/half_h_steps;
        ship.front._yscale=y_sc*coef;

        var hg=y_sc/100*sh_front_h;
        if(ship._y<half_h_steps){
            if(hg<ship_real_h)ship.front._y=hg;
            else ship.front._y=ship_real_h;
            ship.back._y=ship.front._y-hg-sh_back_half_h;
        }
        else{
            ship.front._y=0;
            ship.back._y=hg-sh_back_half_h;
        }
        //cand e sus a trecut jos sau invers, schimb inclinatia
        set_mx_x();
    }

    )");

    //animation

    int wh[2];int img;
    #define celestial_nr 8
    for(int i=0;i<celestial_nr;i++){
        presprite=swf_sprite_new();
        sprintf(buf,"../../img/2/object%u.dbl",i);
        img=swf_image_ex(buf,wh);
        swf_sprite_placeobject(presprite,img,0);
        sprintf(buf,"celestial%u",i);
        sprite=swf_sprite_done(presprite);swf_exports_add(sprite,buf);
    }
    sprintf(buf,"var celestial=new Array('celestial0'");char* ptr=buf;ptr+=strlen(ptr);
    for(int i=1;i<celestial_nr;i++){
        sprintf(ptr,",'celestial%u'",i);
        ptr+=strlen(ptr);
    }
    sprintf(ptr,")");action(buf);

    presprite=swf_sprite_new();
    img=swf_image("../../img/2/speed.dbl");swf_sprite_placeobject(presprite,img,0);
    //fara showframe la gnash nu mai merge onEnterFrame
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"speed");

    actionf(buf,"var celestial_w=%u;var celestial_h=%u",wh[0],wh[1]);

	int scale_frames= 3*fps;
	actionf(buf, "var scale_frames=%u;var scale_frames_celestial=%u", scale_frames, 2* scale_frames);
	//var scale_items=1000;var scale_speed=scale_items;var scale_celestial=8000;
    action(R"(
    //celestial

    var rand_celestial=new Array();
    function add_anim_celestial(){
        celestial_anim.createEmptyMovieClip('celestial_mv',0);
        mc=celestial_anim.celestial_mv;
        set_start(mc,enlarge_celestial,scale_frames_celestial);
        //
        if(rand_celestial.length==0)rand_celestial=celestial.slice();
        var one_celestial_slice=rand_celestial.splice(random(rand_celestial.length),1);
        var one_celestial=one_celestial_slice[0];
        //
        mc.attachMovie(one_celestial,'shape_mc',0);mv=mc['shape_mc'];
        //pun in colturi dar la exterior ca sa nu para ca loveste shipul
        var x_off=-1;var y_off=-1;
        var rand_x=random(2);var rand_y=random(2);
        x_off+=rand_x;y_off+=rand_y;
        x_off*=celestial_w;y_off*=celestial_h;
        rand_x*=width;rand_y*=height;
        x_off+=rand_x;y_off+=rand_y;
        //
        mv._x=x_off;mv._y=y_off;
    }
    function enlarge_celestial(){
        enlarge_an_image(this);
        if(this._xscale>100)this.removeMovieClip();
    }

    //speed
    var last_speed;
    function add_anim_speed(){
        i=0;while(speed_anim.getInstanceAtDepth(i)!=undefined)i++;
        nm='speed'+i;
        speed_anim.attachMovie('speed',nm,i);mv=speed_anim[nm];
        set_start(mv,enlarge_speed,scale_frames);
        last_speed=mv;
    }
    function enlarge_speed(){
        enlarge_an_image(this);
        if(this._xscale>200)this.removeMovieClip();
    }
    )");

    //items

	int bonus_justice_points = 3;
	actionf(buf, "var bonus_justice_number=%u", bonus_justice_points);
	//action asta nu poate avea actionf ca include semnul REST: splits%parts;
	action(R"(
    var min_depth=ship.getDepth();var nextLowestDepth=min_depth-1;

    //var last_unique=0;
    //simplu double numar si are mai multe decat new Date()
    var random_number_mark=new Number(0);
    function unique_mark(){
        //la gnash new Date() e 1970 dar trebuia sa fie today; getTimer nu merge nici la flash nici la gnash
        //var date_mark=new Date();
        //var unique_time=date_mark.valueOf();
        //if(unique_time<=last_unique)unique_time=last_unique+1;last_unique=unique_time;
        random_number_mark++;
        return random_number_mark;
    }
    function enlarge_item(){
        enlarge_an_image(this);
        //ok =
        if(this._xscale>max_scale){this.removeMovieClip();return null;}
        if(this._xscale>100)if(this.getDepth()<ship.getDepth()){
            this.swapDepths(ship);
            var is_hit=0;
            if(this.hitTest(ship._x,ship._y,false)==true)is_hit=1;
            else if(this.hitTest(ship._x+ship_w,ship._y,false)==true)is_hit=1;
            else if(this.hitTest(ship._x,ship._y+ship_real_h,false)==true)is_hit=1;
            else if(this.hitTest(ship._x+ship_w,ship._y+ship_real_h,false)==true)is_hit=1;
            if(is_hit==1){
                if(this.item_type=='cloud')add_splash(ship,this);
                else if(this.item_type=='meteor'){
                    var shields=ship.shields_counted;
                    shield=shields.getInstanceAtDepth(shields.getNextHighestDepth()-1);
                    if(shield!=undefined)shield.removeMovieClip();
                    else add_effect(ship,'boom');
                }
                else{
                    add_effect(ship,'bonus_splash');

                    if(this.item_type=='auto_taker')auto_taker_number=bonus_justice_number;
                    else if(this.item_type=='cloud_to_ship')cloud_to_ship_set();
                    else if(this.item_type=='instant_taker')instant_taker_set();
                    else add_ship_shield();

                    this.removeMovieClip();
                }
            }
        }
    }
    function add_effect(device,eff){
		var mc=add_effect_base(device,eff);
        mc._alpha=50;
    }
	function add_effect_base(device,eff){
        var nx_dp=device.getNextHighestDepth();
        var mc=device.attachMovie(eff,eff+nx_dp,nx_dp);
		return mc;
    }

    var item_w=10*cell;var item_h=item_w;
    //var avoid_center=item_w;
    //var not_viewing=avoid_center*2;
//calculata 200 si vine de 3 ori 600+600+600. 150 si vine de 2 ori 300+(300+300)+300. si tot asa
//height e mai mic decat width, height dispare mai greu ca width
    var max_scale=height/(height-(2*item_w))*100;
//width/not_viewing*100;
    function arrange_depths(){
        if(nextLowestDepth<min_depth){
            i=getNextHighestDepth()-1;
            nextLowestDepth=i+100;
            //
            while(i>=min_depth){
                mv=getInstanceAtDepth(i);
                mv.swapDepths(nextLowestDepth);
                i--;nextLowestDepth--;
            }
        }
    }
    function get_item_and_coords(str,nm){
        arrange_depths();

        createEmptyMovieClip(nm,nextLowestDepth);nextLowestDepth--;
        mc=this[nm];mc['item_type']=str;

        set_start(mc,enlarge_item,scale_frames);

        mc.createEmptyMovieClip('inner_shape',0);mv=mc['inner_shape'];

        //mv._y=random(height-cloud_h);var can_get_center=1;var a_point=height/2-avoid_center-cloud_h;
        //if(mv._y>a_point){a_point=height/2+avoid_center;if(mv._y<a_point)can_get_center=0;}
        //var part_pos=width/2-cloud_w;if(can_get_center==0)part_pos-=avoid_center;mv._x=random(part_pos);if(random(2)==1)mv._x=width-cloud_w-mv._x;
		if(random(2)==1){mv._y=random(height-cloud_h);if(random(2)==1)mv._x=width-cloud_w;}
		else{mv._x=random(width-cloud_w);if(random(2)==1)mv._y=height-cloud_h;}

        return mv;
    }
    var cloud_w=item_w;var cloud_h=item_h;var cloud_part=cloud_w/4;
    var last_cloud;
    function draw_cloud(){
        var nm='cloud'+unique_mark();
        mv=get_item_and_coords('cloud',nm);
        last_cloud=this[nm];

        mv.beginFill(0x00FFff,75);

        var start_x=random(cloud_part);
        var store_3x=3*cloud_part;
        var store_3y=store_3x;

        mv.moveTo(start_x,cloud_part);
        var next_x=cloud_part+random(cloud_part);
        mv.curveTo(cloud_part,0,next_x,cloud_part);
        next_x=2*cloud_part+random(cloud_part);
        mv.curveTo(2*cloud_part,0,next_x,cloud_part);
        var next_y=random(cloud_part);
        mv.curveTo(next_x,0,store_3x,next_y);

        next_y=cloud_part+random(cloud_part);
        mv.curveTo(cloud_w,cloud_part,store_3x,next_y);
        next_y=2*cloud_part+random(cloud_part);
        mv.curveTo(cloud_w,2*cloud_part,store_3x,next_y);
        next_x=cloud_w-random(cloud_part);
        mv.curveTo(cloud_w,next_y,next_x,store_3y);

        next_x=3*cloud_part-random(cloud_part);
        mv.curveTo(3*cloud_part,cloud_h,next_x,store_3y);
        next_x=2*cloud_part-random(cloud_part);
        mv.curveTo(2*cloud_part,cloud_h,next_x,store_3y);
        next_y=cloud_h-random(cloud_part);
        mv.curveTo(next_x,cloud_h,cloud_part,next_y);

        next_y=3*cloud_part-random(cloud_part);
        mv.curveTo(0,3*cloud_part,cloud_part,next_y);
        next_y=2*cloud_part-random(cloud_part);
        mv.curveTo(0,2*cloud_part,cloud_part,next_y);
        mv.curveTo(0,next_y,start_x,cloud_part);

        mv.endFill();

        if(auto_taker_number!=0){add_taker(last_cloud,'auto_taker',auto_taker_return);auto_taker_number--;}
        else if(remaining_cloud_to_ship!=0)cloud__ship();
        else if(remaining_instant_takers!=0)instant__takers();
    }
    //
    var meteor_w=item_w;var meteor_h=item_h;
    var last_meteor;
    function draw_meteors(){
        var nm='meteor'+unique_mark();
        mv=get_item_and_coords('meteor',nm);
        last_meteor=this[nm];

        mv.lineStyle(2,0xff0000);

        var w=width/2;var h=height/2;
        var x_factor=mv._x-w;x_factor=cell*x_factor/w;
        var y_factor=mv._y-h;y_factor=cell*y_factor/h;

        if(mv._x<w)var start_i=0;else var start_i=1;
        if(mv._y<h)var j=0;else var j=1;
        var rows=meteor_w/cell;var cols=meteor_h/cell;
        while(j<rows){
            var y_pos=j*cell;
            i=start_i;
            while(i<cols){
                var x_pos=i*cell;
                k=0;while(k<2){
                    var x=x_pos+random(cell);
                    var y=y_pos+random(cell);
                    mv.moveTo(x,y);mv.lineTo(x+x_factor,y+y_factor);
                    k++;
                }
                i++;
            }
            j++;
        }
    }
    g1=new Array('auto_taker',0x0000FF,10);g2=new Array('cloud_to_ship',0x00ff00,10);g3=new Array('instant_taker',0xffFF00,10);
    s1=new Array('shield_counted',0xff0000,10);
    var bonus_gather_type=new Array(g1,g2,g3);
    var bonus_defense_type=new Array(s1);
    var bonus_w=item_w;

//var qw=0;

    function draw_bonus(){
        //adaug scuturile doar daca nu are
        var bonus_types=bonus_gather_type.slice();
        var shields=ship.shields_counted;
        if(shields.getInstanceAtDepth(shields.getNextHighestDepth()-1)==undefined)bonus_types=bonus_types.concat(bonus_defense_type);

        i=0;nr;max=-1;
        while(i<bonus_types.length){
            b_nr=bonus_types[i][2];
            max_candidate=random(b_nr);
            if(max_candidate>max){nr=i;max=max_candidate;}
            i++;
        }

//if(qw==0){nr=0;qw=1;}
//else{nr=1;qw=0;}

        var one_bonus=bonus_types[nr];
        var splits=one_bonus[2];one_bonus[2]=0;
        var parts=bonus_types.length-1;
        var equal_splits=int(splits/parts);var rest_at=random(parts);
        i=0;while(i<bonus_types.length){
            if(i!=nr){
                if(rest_at==0)bonus_types[i][2]+=splits%parts;
                rest_at--;
                bonus_types[i][2]+=equal_splits;
            }
            i++;
        }

        mv=get_item_and_coords(one_bonus[0],'bonus');var color=one_bonus[1];

//mv._x=width/2;
//mv._y=height/2;

        var sz=10;
        mv.beginFill(0x00ff00,90);
        var main_circle_off=sz+10;var main_circle_r=bonus_w/2-main_circle_off;
        com.drawCircle_offset(mv,main_circle_r,main_circle_off);
        mv.endFill();
        //
        mv.lineStyle(sz,color,90);var color_circle_off=sz/2;
        com.drawCircle_offset(mv,bonus_w/2-color_circle_off,color_circle_off);
    }
    )");

    //bonuses

	actionf(buf, R"(
    //bool
    function a_to_b(a,b,a_w,a_h,b_w,b_h,cloud,device){
        var cloud_last_scale=cloud.last_target_scale;
        var cloud_current_scale=cloud._xscale;
        var c_target_shape=cloud.inner_shape;
        //
        last_left=100-cloud_last_scale;
        calculation=(cloud_current_scale-cloud_last_scale)/last_left;
            //if 99.9 and then 103 and is the first move from instant_taker or cloud_to_ship and dist is big(400) then will be 400*3=1200
        if(cloud_current_scale>100)calculation=1;
        //
        var w_2=a_w/2;var h_2=a_h/2;
        var seek_x=b_w/2+b._x-w_2;var seek_y=b_h/2+b._y-h_2;
        dist_x=seek_x-a._x;dist_y=seek_y-a._y;
        //dif..last_left with x dist
        a._x+=dist_x*calculation;
        a._y+=dist_y*calculation;
        //
        if(cloud_current_scale>100){
            add_splash(device,cloud);
            //inainte erau mai multe bonusuri pe cloud dar acum nu mai trebuie asta            //if(cloud.getDepth()<device.getDepth())
			if(cloud.cloud_to_ship!=true)
			//ca oricum va fi schimbata la principala
				device.swapDepths(cloud);
			else remove_c_to_s();
            cloud.onEnterFrame=enlarge_item;
            return true;
        }
        return false;
    }
    function add_splash(device,cloud){
        add_effect(device,'splash');
            //at cloud_to_ship: reduce alpha, the cloud can come at the ship at the center then into the player eyes, and to remove is not a good idea: can have takers and the play order can conflict with the ship or a taker
            //at takers and normal splash is ok
        cloud._alpha=50;
        _root.counterBar_step();
    }

    function cloud_bonus(){
        //asta trebuie la a_to_b la auto si cloud
        this['last_target_scale']=this._xscale;
        //
        enlarge_an_image(this);
        if(this['auto_taker']==true)taker_seek(this,this['auto_taker'+'seeker']);
        else if(this['cloud_to_ship']==true)a_to_b(this.inner_shape,ship,cloud_w,cloud_h,ship_w,ship_real_h,this,ship);
        else if(this['instant_taker']==true){var s=this['instant_taker'+'seeker'];taker_seek(this,s);instant_taker_connection(s);}
        //
        this['last_target_scale']=this._xscale;
    }

    //taker

    function add_taker(cloud,taker,back_fn){
        d=ship.getDepth();
        upper=getNextHighestDepth()-1;
        while(upper>d){
            mv=getInstanceAtDepth(upper);
            mv.swapDepths(upper+1);
            upper--;
        }
        n=taker+unique_mark();
        attachMovie(taker,n,d+1);
        m=this[n];m._x=ship._x;m._y=ship._y;
        cloud[taker+'seeker']=m;
        cloud[taker]=true;
        cloud.onEnterFrame=cloud_bonus;
        m['back_fn']=back_fn;
    }

    function taker_seek(cloud,seeker){
        if(a_to_b(seeker,cloud.inner_shape,taker_w,taker_h,cloud_w,cloud_h,cloud,seeker)==true){
            seeker.splash._x=(taker_w-splash_w)/2;seeker.splash._y=(taker_h-splash_h)/2;
            seeker.onEnterFrame=seeker.back_fn;
        }
    }
    var back_speed=step;
    function taker_return(seeker){
        dist_x=ship._x-seeker._x;abs_x=Math.abs(dist_x);
        if(abs_x<back_speed)seeker._x=ship._x;
        else seeker._x+=abs_x/dist_x*back_speed;
        //
        dist_y=ship._y-seeker._y;abs_y=Math.abs(dist_y);
        if(abs_y<back_speed)seeker._y=ship._y;
        else seeker._y+=abs_y/dist_y*back_speed;
        //
        if(seeker._x==ship._x)if(seeker._y==ship._y)seeker.removeMovieClip();
    }

    //auto
    var auto_taker_number=0;
    function auto_taker_return(){
        taker_return(this);
    }

    //attract
    var remaining_cloud_to_ship=0;var success_c_to_s;var c_to_s_eff;
    function cloud_to_ship_set(){
        remaining_cloud_to_ship=bonus_justice_number;success_c_to_s=remaining_cloud_to_ship;
		c_to_s_eff=add_effect_base(ship,'c_to_s');
//ca altfel e undefined
		c_to_s_eff._alpha=100;
        cloud__ship();
    }
	function cloud__ship(){
		d=ship.getDepth();
        while(nextLowestDepth<d){
            d--;
            mv=getInstanceAtDepth(d);
            if(mv.item_type=='cloud'){
				if(mv.onEnterFrame!=cloud_bonus){
					set_cloud_to_ship(mv);
					//if(remaining_cloud_to_ship==0)
					return null;
				}
            }
        }
	}
    function set_cloud_to_ship(mv){
        mv.onEnterFrame=cloud_bonus;
        mv['cloud_to_ship']=true;
        remaining_cloud_to_ship--;
    }
	function remove_c_to_s(){
		if(success_c_to_s==1)
			c_to_s_eff.removeMovieClip();
		else{
			success_c_to_s--;
			c_to_s_eff._alpha-=%u;
		}
	}

    //instant
    var remaining_instant_takers=0;
    ship['instant_takers']=new Array();
    function instant_taker_set(){
        remaining_instant_takers=bonus_justice_number;
		instant__takers();
    }
	function instant__takers(){
		depth_value=ship.getDepth();
        while(nextLowestDepth<depth_value){
            depth_value--;
            movie_clip=getInstanceAtDepth(depth_value);
            if(movie_clip.item_type=='cloud'){
				if(movie_clip.onEnterFrame!=cloud_bonus){
					set_instant_taker(movie_clip);
					//if(remaining_instant_takers==0)
					return null;
				}
            }
        }
	}
    function set_instant_taker(cloud){
        add_taker(cloud,'instant_taker',instant_taker_return);
        ship.instant_takers.push(cloud['instant_taker'+'seeker']._name);
        remaining_instant_takers--;
    }

    function instant_taker_return(){
        taker_return(this);
        instant_taker_connection(this);
    }
    function instant_taker_connection(taker){
        var taker_pt_x=taker_w/2;
        var taker_pt_y=taker_h/2;
        var ship_pt_x=ship_w/2+ship._x-taker._x;
        var ship_pt_y=ship_real_h/2+ship._y-taker._y;
        taker.clear();
        taker.lineStyle(2,0x0040FF);
        taker.moveTo(taker_pt_x,taker_pt_y);
        taker.lineTo(ship_pt_x,ship_pt_y);
    }
    function instant_taker_connections_onShipMove(){
        i=0;while(i<ship.instant_takers.length){
            mv_name=ship.instant_takers[i];
            if(this[mv_name]==undefined)ship.instant_takers.splice(i,1);
            else instant_taker_connection(this[mv_name]);
            i++;
        }
    }

    //shield

    function add_ship_shield(){
        var shields=ship.shields_counted;var line_sz=2;var off=line_sz+1;
        i=shields.getNextHighestDepth();
        while(i<bonus_justice_number){
            var sh_name='shield'+i;
            shields.createEmptyMovieClip(sh_name,i);
            shield=shields[sh_name];
            shield.lineStyle(line_sz,0x0080FF,50);
            shield.beginFill(0x0080FF,10);
            //
            var draw_off=1+i*off;
            shield.moveTo(0-draw_off,ship_real_h/2);
            //
            shield.curveTo(0-draw_off,0-draw_off,ship_w/2,0-draw_off);
            shield.curveTo(ship_w+draw_off,0-draw_off,ship_w+draw_off,ship_real_h/2);
            //
            shield.curveTo(ship_w+draw_off,ship_real_h+draw_off,ship_w/2,ship_real_h+draw_off);
            shield.curveTo(0-draw_off,ship_real_h+draw_off,0-draw_off,ship_real_h/2);
            //
            shield.endFill();
            //
            i++;
        }
    }
    )", 100/bonus_justice_points);

    //
	//var min=1;var x_sc=img._xscale*img._xscale/scale_factor;if(x_sc<min)x_sc=min;img._xscale+=x_sc;var y_sc=img._yscale*img._yscale/scale_factor;if(y_sc<min)y_sc=min;img._yscale+=y_sc;
	action(R"(
    function set_start(mv,fn,scale_factor){
        mv._xscale=0;mv._yscale=0;
        mv.onEnterFrame=fn;
        mv['scale_factor']=scale_factor;
		mv['scale_frame']=0;
    }
    function enlarge_an_image(img){
		img.scale_frame++;
		//frama curenta/toate framele
		var frames_factor=img.scale_frame/img.scale_factor;
		//din 100%scale cat e in frama asta
		var sc=frames_factor*100;
		//inca o divizare(frames_factor e subunitar) ca la inceput trebuie incet, la sfarsit trebuie repede: 50 vine 25, 100 este 100
		//inca cateva ca sa fie mai realist
		var scale_factor=sc*Math.pow(frames_factor,3);
		img._xscale=scale_factor;
		img._yscale=scale_factor;

        var wd=width*img._xscale/100;
        img._x=(width-wd)/2;

        var hg=height*img._yscale/100;
        img._y=(height-hg)/2;
    }

    var bonus_frames=0;var bs_time=fps*2;
    //var slow_see=fps;
    function frame_enter(){
//slow_see--;if(slow_see!=0){return undefined;}slow_see=fps;
        if(celestial_anim.celestial_mv==undefined)add_anim_celestial();
        //cu php e facuta width/4 in stanga, lafel in dreapta, height/4 sus, lafel jos. nu mai e timp de calculat exact
//number:1.8838e-005  [number:0.000301408] [number:0.00152588 number:0.00482253 :0.01177 number:0.02441 number:0.0452
//dispare in 3 si ceva , doua si ceva pe frame, 9 si ceva in total
        if(last_speed._xscale>0.03)add_anim_speed();
        //sa nu intre ambele pe aceeasi frama ca intra in conflict cu swapDepths daca nu sunt una dupa alta
        if(random(10)+5<last_cloud._xscale)draw_cloud();
        else if(random(10)+5<last_meteor._xscale)draw_meteors();
        else if(bonus==undefined){
            if(bs_time+random(bs_time)<bonus_frames){draw_bonus();bonus_frames=0;}
            bonus_frames++;
        }
        //
        move_ship();
    }
    add_anim_speed();
//sa nu intre ambele pe aceeasi frama ca intra in conflict cu swapDepths daca nu sunt una dupa alta
    draw_cloud();
	function safe_enter(){
		draw_meteors();
		onEnterFrame=frame_enter;
	}
    onEnterFrame=safe_enter;
    _root.counterBar_init(50);
    )");

    //splash
    int splash_sz[2];
    presprite=swf_sprite_new();bmp=swf_image_ex("C:/Desktop/universe/img/2/splash.dbl",splash_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/2;i++)swf_sprite_showframe(presprite);
    action_sprite(presprite,"removeMovieClip()");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"splash");

    actionf(buf,"var splash_w=%u;var splash_h=%u",splash_sz[0],splash_sz[1]);

    //boom
	action("var boom_stop=0");
    presprite=swf_sprite_new();bmp=swf_image("C:/Desktop/universe/img/2/boom.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
	int frames_stop = fps * 3;
	action_sprite(presprite, "_root['episode'].boom_stop++");
	for (int i = 0; i < frames_stop; i++)swf_sprite_showframe(presprite);
	action_sprite(presprite, "_root['episode'].boom_stop--;removeMovieClip()");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"boom");

    //bonus_splash
    presprite=swf_sprite_new();bmp=swf_image("C:/Desktop/universe/img/2/bonus_splash.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/2;i++)swf_sprite_showframe(presprite);
    action_sprite(presprite,"removeMovieClip()");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bonus_splash");

	//c_to_s
	presprite = swf_sprite_new();
	swf_sprite_placeobject(presprite, bmp, 0);
	/*action_sprite(presprite, R"(
cacheAsBitmap=true;
var matrix=new Array();
//red
matrix=matrix.concat(1,0,0,0,0);
//green
matrix=matrix.concat(0,1,0,0,0);
//blue
matrix=matrix.concat(0,0,0,0,0);
//alpha
matrix=matrix.concat(0,0,0,1,0);
var filter=new flash.filters.ColorMatrixFilter(matrix);
filters=new Array(filter);
)");*/
	swf_sprite_showframe(presprite);
	sprite = swf_sprite_done(presprite); swf_exports_add(sprite, "c_to_s");

    int taker_sz[2];

    //auto_taker
    presprite=swf_sprite_new();
    //
    bmp=swf_image_ex("C:/Desktop/universe/img/2/auto_taker1.dbl",taker_sz);
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/2/auto_taker2.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    swf_sprite_removeobject(presprite,0);
    bmp=swf_image("C:/Desktop/universe/img/2/auto_taker3.dbl");
    swf_sprite_placeobject(presprite,bmp,0);
    for(int i=0;i<fps/3;i++)swf_sprite_showframe(presprite);
    //
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"auto_taker");

    actionf(buf,"var taker_w=%u;var taker_h=%u",taker_sz[0],taker_sz[1]);

    //instant_taker
    presprite=swf_sprite_new();
    //
    bmp=swf_image("C:/Desktop/universe/img/2/instant_taker.dbl");swf_sprite_placeobject(presprite,bmp,0);
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"instant_taker");

    swf_exports_done();
    swf_showframe();
    swf_done();
	printf("\ndone");
    return 1;
}

#ifdef __cplusplus
}
#endif
