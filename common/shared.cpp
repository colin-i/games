



#include <Windows.h>
#include<stdio.h>
//#include <Math.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

//circle
//fuel
//rotation bmp
//space_env
//so_keys
void shared_write_ex(bool circle,int fuel,int rotation,bool space_env,bool keys){
    presprite=swf_sprite_new();

	if(circle){
	//circle
    action_sprite(presprite,R"(
    function drawCircle_offset(mc,r,off){
        drawCircle_coords(mc,r,off,off);
    }
    function drawCircle_coords(mc,r,xPos,yPos){
        mc.moveTo(r+r+xPos,r+yPos);
        mc.curveTo(r+r+xPos,Math.tan(Math.PI/8)*r+r+yPos,Math.sin(Math.PI/4)*r+r+xPos,Math.sin(Math.PI/4)*r+r+yPos);
        mc.curveTo(Math.tan(Math.PI/8)*r+r+xPos,r+r+yPos,r+xPos,r+r+yPos);
        mc.curveTo(0-Math.tan(Math.PI/8)*r+r+xPos,r+r+yPos,0-Math.sin(Math.PI/4)*r+r+xPos,Math.sin(Math.PI/4)*r+r+yPos);
        mc.curveTo(xPos,Math.tan(Math.PI/8)*r+r+yPos,xPos,r+yPos);
        mc.curveTo(xPos,0-Math.tan(Math.PI/8)*r+r+yPos,0-Math.sin(Math.PI/4)*r+r+xPos,0-Math.sin(Math.PI/4)*r+r+yPos);
        mc.curveTo(0-Math.tan(Math.PI/8)*r+r+xPos,yPos,r+xPos,yPos);
        mc.curveTo(Math.tan(Math.PI/8)*r+r+xPos,yPos,Math.sin(Math.PI/4)*r+r+xPos,0-Math.sin(Math.PI/4)*r+r+yPos);
        mc.curveTo(r+r+xPos,0-Math.tan(Math.PI/8)*r+r+yPos,r+r+xPos,r+yPos);
    }
    )");
	}
	if(fuel!=0){
	//fuel
	actionf_sprite(presprite, buf, "var fps=%u", fps);
	action_sprite(presprite, R"(
	function add_fuel_main(width_size,height_size,sense,name,container,at_center,default_angle){
		var mc=container.createEmptyMovieClip(name,container.getNextHighestDepth());
        mc.width_size=width_size;

        mc.colors=new Array(0xff0000,0xffFF00);
        //
        mc.alphas=new Array(80,80);
        //folosesc setMember;new Array(200) e push 200,push 1,push 'Array',newobj  si la enumerare e nimic la flash player
        mc.ratios=new Array();mc.ratios[0]=0;
        //e jumate din width size daca e mare cum e la width=40(sa fie mai mult de un sfert heightul) sau 10
        var nr=Math.max(width_size/2,10);
        //
        mc.interval_one_h=height_size*2-(nr/2);mc.interval_two_h=mc.interval_one_h+nr;
        mc.step=(mc.interval_two_h-mc.interval_one_h)/(fps/8);
        mc.interval_one_h*=sense;mc.interval_two_h*=sense;mc.step*=sense;
        mc.current_h=mc.interval_one_h;mc.next_interval=mc.interval_two_h;
        mc.fuel_matrix=new flash.geom.Matrix();
        //
        if(at_center!=true)mc.fuel_matrix.tx=mc.width_size/2;
        else{mc['at_center']=at_center;mc['default_angle']=default_angle;}
        //
        mc.fuelOn=function(){
			var a=this.current_h<this.next_interval;
            this.current_h+=this.step;
            if(a!=(this.current_h<this.next_interval)){
				if(this.next_interval==this.interval_one_h)this.next_interval=this.interval_two_h;
                else this.next_interval=this.interval_one_h;
                this.step*=-1;
            }
			//current_h e varful curvei unde nu ajunge desenul,las la o treime
            this.ratios[1]=255*(Math.abs(this.current_h)/3)/Stage.width;
            this.clear();
            this.beginGradientFill('radial',this.colors,this.alphas,this.ratios,this.fuel_matrix);
            //
            if(this.default_angle!=true){
                if(this.at_center!=true){
                    this.lineTo(this.width_size,0);this.curveTo(this.width_size/2,this.current_h,0,0);
                }else{
                    this.moveTo(0-(this.width_size/2),0);
                    this.lineTo(this.width_size/2,0);this.curveTo(0,this.current_h,0-(this.width_size/2),0);
                }
            }else{
                this.moveTo(0,0-(this.width_size/2));
                this.lineTo(0,this.width_size/2);this.curveTo(0-this.current_h,0,0,0-(this.width_size/2));
            }
            //
            this.endFill();
        }
    }
	)");
	if (fuel & 1)action_sprite(presprite, "function add_fuel(width_size,height_size,sense,name,container){add_fuel_main(width_size,height_size,sense,name,container);}");
	if (fuel & 2)action_sprite(presprite, "function append_fuel(width_size,height_size,name,container){add_fuel_main(width_size,height_size,1,name,container,true,true);}");
	if (fuel & 4)action_sprite(presprite, "function addFuel(width_size,height_size,sense,name,container){add_fuel_main(width_size,height_size,sense,name,container,true);}");
	}
	if(rotation!=0){
    //rotation bmp
	action_sprite(presprite, R"(
	var BitmapData=flash.display.BitmapData;
	function rotate_bmp(){
        this.mx.rotate(this.rot*Math.PI/180);
        var wd=this.bmp.width/2;var hg=this.bmp.height/2;
        this.clear();
        this.beginBitmapFill(this.bmp,this.mx,false);
        this.moveTo(0-wd,0-hg);
        this.lineTo(wd,0-hg);this.lineTo(wd,hg);
        this.lineTo(0-wd,hg);this.lineTo(0-wd,0-hg);
        this.endFill();
    }
	)");
	if (rotation & 7) {
	action_sprite(presprite, R"(
	//mc
    function mc_rotation_bmp_coords(bmp_pre,container,name,dp,rot_step,x_val,y_val){
        //pun margini transparente ca altfel: When repeat is set to false, the bitmap fill uses the edge pixels for the fill area outside of the bitmap
        var fix_margins=2;
        var bmp=new BitmapData(bmp_pre.width+fix_margins,bmp_pre.height+fix_margins,true,0);
        var sourceRect=new flash.geom.Rectangle(0,0,bmp_pre.width,bmp_pre.height);
        var destPoint=new flash.geom.Point(1,1);
        bmp.copyPixels(bmp_pre,sourceRect,destPoint);
        //
        var mc=container.createEmptyMovieClip(name,dp);
        mc.mx=new flash.geom.Matrix();
        mc.mx.translate(0-(bmp.width/2),0-(bmp.height/2));
        mc.bmp=bmp;mc.rot=rot_step;
        //
        mc._x=x_val;mc._y=y_val;
        //
        mc.rotate_bmp=rotate_bmp;
        delete bmp_pre;
    }
	)");
	}
	if (rotation & 6) {
	action_sprite(presprite, R"(
	function mc_rotation_bmp(bmp_pre,container,name,dp,rot_step){
		mc_rotation_bmp_coords(bmp_pre,container,name,dp,rot_step,bmp_pre.width/2,bmp_pre.height/2);
	}
	)");
	}
	if (rotation & 4) {
		action_sprite(presprite, R"(
    function mc_rotation_bmp_show(in_bmp,container,name,dp,rot_step){
        mc_rotation_bmp(in_bmp,container,name,dp,rot_step);
        container[name].rotate_bmp();
    }
    )");
	}
	if (rotation & 8) {
	action_sprite(presprite, R"(
	function rotate_mc_andChangeStep(mc,value){
        mc.rot=value;mc.rotate_bmp();
    }
	)");
	}
	}
	if(space_env){
    //space_env
    action_sprite(presprite,"attachMovie('space_env','space_env',getNextHighestDepth())");
    //
    int presprite_space_env=swf_sprite_new();
    //
    dbl=swf_dbl("C:/Desktop/universe/img/5/env.dbl");
    swf_exports_add(dbl,"space_env_bmp");
    int star_sz[2];
    dbl=swf_dbl_ex("C:/Desktop/universe/img/5/star.dbl",star_sz);
    swf_exports_add(dbl,"space_star_bmp");
    //
    actionf_sprite(presprite_space_env,buf,"var width=%u;var height=%u",width,height);
    actionf_sprite(presprite_space_env,buf,"var star_w=%u;var star_h=%u",star_sz[0],star_sz[1]);
    action_sprite(presprite_space_env,R"(
        function space_env(container,name,depth){
            var mv=container.createEmptyMovieClip(name,depth);
            var mx=new flash.geom.Matrix();
            mv.beginBitmapFill(space_env_bmp,mx,true);
            mv.lineTo(width,0);mv.lineTo(width,height);
            mv.lineTo(0,height);mv.lineTo(0,0);
            mv.endFill();
            //
            var max_cols=3;var max_rows=3;var cols_ratio=width/max_cols;var rows_ratio=height/max_cols;
            var row=0;
            while(row<max_rows){
                var col=0;while(col<max_cols){
                    var d=mv.getNextHighestDepth();
                    //
                    var mc=mv.createEmptyMovieClip('star'+d,d);
                    mc.beginBitmapFill(space_star_bmp,mx,false);
                    mc.lineTo(star_w,0);mc.lineTo(star_w,star_h);
                    mc.lineTo(0,star_h);mc.lineTo(0,0);
                    mc.endFill();
                    //
                    mc._x=col*cols_ratio+random(cols_ratio);
                    mc._y=row*rows_ratio+random(rows_ratio);
                    col++;
                }
                row++;
            }
        }
		//ca sa fie mai rapid
        _root['shared_level'].space_env=space_env;
        var space_env_bmp=flash.display.BitmapData.loadBitmap('space_env_bmp');
        var space_star_bmp=flash.display.BitmapData.loadBitmap('space_star_bmp');
    )");swf_sprite_showframe(presprite_space_env);
    sprite=swf_sprite_done(presprite_space_env);swf_exports_add(sprite,"space_env");
	}
	if(keys){
    //so_keys
    action_sprite(presprite,R"(
        var so_keys=SharedObject.getLocal('keys');
        var i=0;while(i<_root.key_names.length){
            var key_id=_root.key_names[i];
            if(so_keys.data[key_id]!=undefined)this[key_id]=so_keys.data[key_id];
            i++;
        }
    )");
	}

    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"shared_level");
}
void shared_write() { shared_write_ex(true, 7, 15, true, true); }
#ifdef __cplusplus
}
#endif
