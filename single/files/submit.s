
function submit_form(sd deffont,sd width,sd height,sd game_name)
ss submitform_string
ss extra_string
sd f
importx "_open" open
importx "_close" close
setcall f open("./kong_build.txt",(0|0x8000))
if f==-1
    str submitexport="submit"

    sd submitsprite
    setcall submitsprite swf_sprite_new()

    #all to resolve here is if "deffont" is in the swf or is not in the swf
    #width and height are game constants
    #another in: game_name

    const submit_width=540
    const submit_height=380
    const margin=40
    const both_margins=2*margin
    const button_width=150
    const entry_height=50
    const left_name=100
    const text_font_height=20
    const input_width=submit_width-both_margins-left_name
    const input_left=margin+left_name

    sd submitshape
    setcall submitshape swf_shape_basic((submit_width),(submit_height),0xff0011ff,0x00FF01ff)
    call swf_sprite_placeobject(submitsprite,submitshape,1)

    sd name
    sd score
    sd comment
    sd submit
    sd newgame
    sd submitform_y=20
    sd e_strct
    setcall e_strct edittext_struct()
    sd text_handler

    add submitform_y 20
    setcall text_handler static_text((left_name),"Name",deffont,(text_font_height))
    call swf_sprite_placeobject_coords(submitsprite,text_handler,2,(margin),submitform_y)
    call edittext_font(deffont,40)
    setcall name swf_text((input_width),(entry_height),"name_text",(HasFont|Border),e_strct)
    call swf_sprite_placeobject_coords(submitsprite,name,3,(input_left),submitform_y)
    add submitform_y (entry_height)
    #
    add submitform_y 20
    setcall text_handler static_text((left_name),"Score",deffont,(text_font_height))
    call swf_sprite_placeobject_coords(submitsprite,text_handler,4,(margin),submitform_y)
    call edittext_font(deffont,40)
    setcall score swf_text((input_width),(entry_height),"score_text",(HasFont|Border|ReadOnly),e_strct)
    call swf_sprite_placeobject_coords(submitsprite,score,5,(input_left),submitform_y)
    add submitform_y (entry_height)
    #
    const comment_sz=2*entry_height
    add submitform_y 20
    setcall text_handler static_text((left_name),"Comment",deffont,(text_font_height))
    call swf_sprite_placeobject_coords(submitsprite,text_handler,6,(margin),submitform_y)
    call edittext_font(deffont,30)
    setcall comment swf_text((input_width),(comment_sz),"comment_text",(HasFont|Border|Multiline|WordWrap),e_strct)
    call swf_sprite_placeobject_coords(submitsprite,comment,7,(input_left),submitform_y)
    add submitform_y (comment_sz)
    #
    add submitform_y 20

    chars ac_data#8192
    str ac^ac_data

    call sprintf(ac,"
    var game_name='%s';
    getURL('http://oa.netau.net/games/post.php','_self','POST')
    ",game_name)
        chars ButtonData_a="Submit"
                    #            8|                    8|            8|   8|
    data ButtonData={0x11FF11ff,10,0xee1122ff,0xaaFF11ff,10,0x883344ff,24,24}
    str *^ButtonData_a;str ButtonData_b#:/DWORD-1;if :==(QWORD);set ButtonData_b 0;endif
        data ButtonData_c#1;set ButtonData_c deffont;data *=text_font_height
        #vert off is one quarter; raise the text one quarter
        data vert_off#1;set vert_off (text_font_height);div vert_off 4;mult vert_off -1
        data *font_color=0xff
    str ButtonData_d#1;str ButtonData_e#:/DWORD-1;set ButtonData_d ac;if :==(QWORD);set ButtonData_e 0;endif
    setcall submit swf_button((button_width),(entry_height),#ButtonData)
    call swf_sprite_placeobject_coords(submitsprite,submit,8,(margin),submitform_y)
    setcall newgame swf_button_last("New Game","_root.newgame();removeMovieClip()")
    call swf_sprite_placeobject_coords(submitsprite,newgame,9,(submit_width-button_width-margin),submitform_y)

    sd submitform
    call swf_sprite_showframe(submitsprite)
    setcall submitform swf_sprite_done(submitsprite)
    call swf_exports_add(submitform,submitexport)

    sub width (submit_width)
    div width 2

    sub height (submit_height)
    div height 2

    chars submitform_chars#500
    str submitform_full^submitform_chars
    call sprintf(submitform_full,"attachMovie('submit','_submit',getNextHighestDepth());
_submit._x=%u;
_submit._y=%u;
_submit.score_text=score",width,height)
    set submitform_string submitform_full
    str blank=""
    set extra_string blank
else
    call close(f)
    call action("
        var kongregate=_global.kongregate;
        kongregate.services.connect();
        function to_kongregate(score)
        {
            send_score_kong(score);
            _text.message='Submitted score: '+score+'. Click to play again';
            onMouseDown=kong_newgame;
        }
        function no_function(){}
        function kong_newgame()
        {
            onMouseDown=no_function;
            newgame();
        }
        function send_score_kong(score)
        {
            kongregate.stats.submit('Highscore',score);
        }
    ")
    str submitform_string_kong="to_kongregate(score)"
    set submitform_string submitform_string_kong
    ss extra_string_kong="send_score_kong(score);"
    set extra_string extra_string_kong
endelse

call actionf(ac,"
function submitform(score)
{
    score=int(score);
    %s;
}
",submitform_string)
call actionf(ac,"function update_net_score(score){%s}",extra_string);
endfunction
