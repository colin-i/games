
#include<windows.h>
#include<stdio.h>

#include "c:/desktop/universe/action swf/actionswf/include/actionswf.h"
#include "../inc/universe.h"

#ifdef __cplusplus
extern "C" {
#endif

void _exec(){
    swf_new("com.swf",width,height,0x112211,fps);

    shared_write();

    action(R"(

    //System.security.allowDomain('http://89.35.62.10');
    //System.security.allowDomain('http://89.35.62.10/test/');
    //System.security.allowDomain('http://89.35.62.10/test');
    //System.security.allowDomain('http://89.35.62.10/test/ep.swf');
    //System.security.allowDomain('http://universe-game.com');
    //System.security.allowDomain('http://universe-game.com/test');
    //System.security.allowDomain('http://universe-game.com/test/ep.swf');
        attachMovie('shared','shared_level',getNextHighestDepth());
        var q=1;onEnterFrame=function(){
            q++;if(q==3){
                onEnterFrame=undefined;

		var load_vars=new LoadVars();
		
load_vars.onLoad=function(success){
			if(success==true){
		                createEmptyMovieClip('scenario',getNextHighestDepth());
		                //scenario.loadMovie('ep.swf');
		                //scenario.loadMovie('http://universe-game.com/test/ep.swf');
				var nr=this.nr;
		                scenario.loadMovie('ep'+nr+'.swf');
			}
		
}
		
load_vars.load("nr.txt");
            }
        }
    )");
    swf_exports_done();
    swf_showframe();
    swf_done();
    return;
}

#ifdef __cplusplus
}
#endif
