
sd debpre
sd debsp

setcall debpre swf_sprite_new()

call swf_sprite_showframe(debpre)

call action_sprite(debpre,"_root.mixt.onPress()");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.move_turret_coords(200,20)");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.set_turret()");call swf_sprite_showframe(debpre)

call action_sprite(debpre,"_root.mixt.onPress()");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.move_turret_coords(280,20)");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.set_turret()");call swf_sprite_showframe(debpre)

call action_sprite(debpre,"_root.mixt.onPress()");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.move_turret_coords(200,60)");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.set_turret()");call swf_sprite_showframe(debpre)

call action_sprite(debpre,"_root.mixt.onPress()");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.move_turret_coords(280,60)");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.set_turret()");call swf_sprite_showframe(debpre)

call action_sprite(debpre,"_root.mixt.onPress()");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.move_turret_coords(200,100)");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.set_turret()");call swf_sprite_showframe(debpre)

call action_sprite(debpre,"_root.mixt.onPress()");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.move_turret_coords(280,100)");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.set_turret()");call swf_sprite_showframe(debpre)

call action_sprite(debpre,"_root.mixt.onPress()");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.move_turret_coords(200,140)");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.set_turret()");call swf_sprite_showframe(debpre)

call action_sprite(debpre,"_root.mixt.onPress()");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.move_turret_coords(280,140)");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.set_turret()");call swf_sprite_showframe(debpre)

call action_sprite(debpre,"_root.mixt.onPress()");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.move_turret_coords(200,180)");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.set_turret()");call swf_sprite_showframe(debpre)

call action_sprite(debpre,"_root.air.onPress()");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.move_turret_coords(320,140)");call swf_sprite_showframe(debpre)
call action_sprite(debpre,"_root.set_turret()");call swf_sprite_showframe(debpre)

call init_debug_grid()

call action_sprite(debpre,"stop();
//road.air0.stop();
//_root.debug_grid(_root.road,_root.grid_lat,_root.grid_lat,_root.map);");call swf_sprite_showframe(debpre)

setcall debsp swf_sprite_done(debpre)
call swf_exports_add(debsp,"db")
call action("attachMovie('db','db',1000);
function turret_debug(ar)
{
    var x='';
    var i=0;while(i<ar.length){
        var z=ar[i];
        x=x+'                ~'+z[0];
        var j=0;while(j<z[1].length){
            x=x+'!'+z[1][j];j++;
        }
        i++;
    }
    dbg_text.dbg_text=x;
    dbg_text._visible=true;
}
")

setcall presprite swf_sprite_new()
sd e_strct
setcall e_strct edittext_struct()
sd dbg_text
setcall dbg_text swf_text(500,1000,"dbg_text",(Border|Multiline|WordWrap),e_strct)
call swf_sprite_placeobject(presprite,dbg_text,0)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"dbg_text")
call action("attachMovie('dbg_text','dbg_text',900);dbg_text._visible=false;")
