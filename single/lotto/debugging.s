
    #5
call action_sprite(presprite,"
_x=100;_y=100;_root.new_sense_set(this,3,1,0);
_root._text.message=on_x+' '+on_y;
")
call swf_sprite_showframe(presprite)
    #
sd j=0
while j<120
call action_sprite(presprite,"_x=_x+on_x;_y=_y+on_y")
call swf_sprite_showframe(presprite)
inc j
endwhile
    #
call action_sprite(presprite,"stop()")
call swf_sprite_showframe(presprite)
