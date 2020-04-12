

#text

importx "sprintf" sprintf
importx "strlen" strlen
importx "strcat" strcat
importx "memcmp" memcmp

import "edittext_struct" edittext_struct
import "edittext_font" edittext_font
import "edittext_rgba" edittext_rgba
import "edittext_text" edittext_text

const cell=20
const font_rect_width=2000
const font_rect_height=50
#depths
function default_text(sd depths,sd message_color,sd deffont,sd y)
    setcall depths default_text_withHeight(depths,message_color,deffont,y,(font_rect_height))
    return depths
endfunction
const static_text_off_add=10
#depths
function default_text_withHeight(sd depths,sd message_color,sd deffont,sd y,sd height)
    #_root._text.message
    sub height (static_text_off_add)
    #
    chars data#256
    str str^data

    ss textnm
    setcall textnm texter_name()

    sd textmovieid
    setcall textmovieid swf_sprite_new()
    #
    sd ed_str
    setcall ed_str edittext_struct()
    call edittext_font(deffont,height)
    call edittext_rgba(message_color)
    sd txt
    ss msg
    setcall msg texter_msg()
    setcall txt swf_text((font_rect_width),(font_rect_height),msg,(HasFont|HasTextColor|ReadOnly|NoSelect),ed_str)
    call swf_sprite_placeobject(textmovieid,txt,1)
    call swf_sprite_showframe(textmovieid)
    #
    sd textmovie
    setcall textmovie swf_sprite_done(textmovieid)
    call swf_exports_add(textmovie,textnm)
    if depths!=-1
        call sprintf(str,"attachMovie('%s','%s',%u);_text._y=%u",textnm,textnm,depths,y)
    else
        call sprintf(str,"attachMovie('%s','%s',getNextHighestDepth());_text._y=%u",textnm,textnm,y)
    endelse
    call action(str)
    inc depths
    return depths
endfunction
function default_text_ex(sd message_color,sd deffont,sd y,sd height)
    call default_text_withHeight(-1,message_color,deffont,y,height)
endfunction
#name
function texter_name()
    return "_text"
endfunction
#name
function texter_msg()
    return "message"
endfunction

#id
function static_text(sd bound_width,ss initialtext,sd font_id,sd font_height)
    call edittext_font(font_id,font_height)
    call edittext_text(initialtext)
    sd e_struct
    setcall e_struct edittext_struct()
        #add some pixels to see all the text
    add font_height (static_text_off_add)
    sd text_id
    setcall text_id swf_text(bound_width,font_height,"",(HasFont|HasText|ReadOnly|NoSelect),e_struct)
    return text_id
endfunction
#id
function dyn_text(sd bound_width,ss variabletext,sd font_id,sd font_height)
    call edittext_font(font_id,font_height)
    sd e_struct
    setcall e_struct edittext_struct()
        #add some pixels to see all the text
    add font_height (static_text_off_add)
    sd text_id
    setcall text_id swf_text(bound_width,font_height,variabletext,(HasFont|ReadOnly|NoSelect),e_struct)
    return text_id
endfunction
#id
function dyn_text_color(sd bound_width,ss variabletext,sd font_id,sd font_height,sd color)
    call edittext_font(font_id,font_height)
    sd e_struct
    setcall e_struct edittext_struct()
    call edittext_rgba(color)
    #add some pixels to see all the text
    add font_height (static_text_off_add)
    sd text_id
    setcall text_id swf_text(bound_width,font_height,variabletext,(HasFont|ReadOnly|NoSelect|HasTextColor),e_struct)
    return text_id
endfunction

#grids


import "error" error
function declare_grid_zero(sd width,sd height,ss grid_name,sd sprite)
    call declare_grid(width,height,grid_name,sprite,"0")
endfunction
function declare_grid(sd width,sd height,ss grid_name,sd sprite,ss value)
    if width==0
        call error("no width???")
    endif
    if height==0
        call error("no height???")
    endif
    #[row][col]
    chars c#8192
    str str^c
    #actionscript the col
    str col_frm="var %s_%u=new Array("
    sd row=0
    chars cols_again#256;str cols_f^cols_again
    ss ptr
    sd cmp;str und="undefined";sd len;setcall len strlen(und);setcall cmp memcmp(value,und,len)
    while row<height
        set c 0
        call sprintf(str,col_frm,grid_name,row)
        set ptr str
        addcall ptr strlen(str)
        #
        if cmp!=0
            chars cm=Comma
            set ptr# 0;call strcat(ptr,value);addcall ptr strlen(ptr)
            sd col=1
            while col<width
                set ptr# cm
                inc ptr
                set ptr# 0;call strcat(ptr,value);addcall ptr strlen(ptr)
                inc col
            endwhile
        endif
        #
        set ptr# (Closeparenthesis)
        inc ptr
        set ptr# (Colon)
        inc ptr
        set ptr# 0
        if sprite==0
            call action(str)
        else
            call action_sprite(sprite,str)
        endelse
        if cmp==0
            call sprintf(cols_f,",%s_%u.length=%u;",grid_name,row,width)
            if sprite==0
                call action(cols_f)
            else
                call action_sprite(sprite,cols_f)
            endelse
        endif
        inc row
    endwhile
    #and the rows
    str row_frm="var %s=new Array("
    set c 0
    call sprintf(str,row_frm,grid_name)
    set ptr str
    addcall ptr strlen(str)
    set cols_again 0
    call sprintf(cols_f,"%s_0",grid_name)
    call strcat(ptr,cols_f)
    addcall ptr strlen(cols_f)
    set row 1
    while row<height
        set cols_again 0
        call sprintf(cols_f,",%s_%u",grid_name,row)
        call strcat(ptr,cols_f)
        addcall ptr strlen(cols_f)
        inc row
    endwhile
    set ptr# (Closeparenthesis)
    inc ptr
    set ptr# (Colon)
    inc ptr
    set ptr# 0
    if sprite==0
        call action(str)
    else
        call action_sprite(sprite,str)
    endelse
endfunction

function init_reset_grid()
    call action("
    function reset_grid(width,height,grid_param)
    {
        var row=0;
        while(row<height)
        {
            var col=0;
            while(col<width){
                grid_param[row][col]=0;
                col=col+1;
            }
            row=row+1;
        }
    }")
endfunction
function init_reset_grid_value()
    call action("
    function reset_grid_value(width,height,grid_param,value)
    {
        var row=0;
        while(row<height)
        {
            var col=0;
            while(col<width){
                grid_param[row][col]=value;
                col=col+1;
            }
            row=row+1;
        }
    }")
endfunction

#grids debug

function init_debug_grid()
call action("
function debug_grid(container,width,height,grid)
{
    if(container.dbg==undefined)
    {
        container.createEmptyMovieClip('dbg',container.getNextHighestDepth());
        var dbg_txt_x;
        var dbg_txt_y=0;
        var row=0;var col;
        var i=0;
        while(row<height)
        {
            col=0;
            dbg_txt_x=0;
            while(col<width){
                var debug_name='debg_'+row+'_'+col;
                container.dbg.createTextField(debug_name,container.dbg.getNextHighestDepth(),dbg_txt_x,dbg_txt_y,cell,cell);
                col=col+1;
                dbg_txt_x=dbg_txt_x+cell;
            }
            row=row+1;
            dbg_txt_y=dbg_txt_y+cell;
        }
    }
    row=0;
    while(row<height)
    {
        col=0;
        while(col<width){
            debug_name='debg_'+row+'_'+col;
            container.dbg[debug_name].text=grid[row][col];
            col=col+1;
        }
        row=row+1;
    }
}
function debug_arraygrid(container,width,height,arraygrid)
{
    if(container.dbg==undefined)
    {
        container.createEmptyMovieClip('dbg',container.getNextHighestDepth());
        var dbg_txt_x;
        var dbg_txt_y=0;
        var row=0;var col;
        var i=0;
        while(row<height)
        {
            col=0;
            dbg_txt_x=0;
            while(col<width){
                var debug_name='debg_'+row+'_'+col;
                container.dbg.createTextField(debug_name,container.dbg.getNextHighestDepth(),dbg_txt_x,dbg_txt_y,cell,cell);
                col=col+1;
                dbg_txt_x=dbg_txt_x+cell;
            }
            row=row+1;
            dbg_txt_y=dbg_txt_y+cell;
        }
    }
    row=0;
    while(row<height)
    {
        col=0;
        while(col<width){
            debug_name='debg_'+row+'_'+col;
            container.dbg[debug_name].text='-';
            col=col+1;
        }
        row=row+1;
    }
    var i=0;var back;
    while(i<arraygrid.length)
    {
        var pos=arraygrid[i];
        row=pos/width;row=int(row);back=row*width;
        col=pos-back;

        debug_name='debg_'+row+'_'+col;
        container.dbg[debug_name].text=pos;

        i=i+1;
    }
}
")
endfunction

function rest(sd nr,sd div)
    sd x
    set x nr
    div x div
    mult x div
    sub nr x
    return nr
endfunction
