setcall presprite swf_sprite_new()
#1
call action("var iter=_grid1_map._y")
call init_debug_grid()
#
sd q=0
while q<4
call action("_boats.hz_dragShip()")
call action("
current_inst._x=0-deltaX;
current_inst._y=iter;
iter=iter+cell;
move_ship();")
call swf_showframe()
#
call action("
place_ship();
")
call swf_showframe()
inc q
endwhile

#
call action("
//_grid2_map.ai_init();
//_grid2_map.comp_field_init();
")
call swf_showframe()
#
set q 0
while q<0
sd wer=0
while wer>0
    call swf_showframe()
    dec wer
endwhile
call action("
_grid2_map.ai_move();
//_root.debug_grid(_root._grid2_map,_root.grid_width,_root.grid_height,_root.comp_grid);
")
call swf_showframe()
inc q
endwhile

call action("
stop()
")

call swf_showframe()
