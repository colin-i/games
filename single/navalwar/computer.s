
#computer placing boats

ss comp_grid_hz_test="comp_grid_hz_test"
ss comp_grid_vt_test="comp_grid_vt_test"
call declare_grid_zero(grid_width,grid_height,comp_grid_hz_test,presprite)
call declare_grid_zero(grid_width,grid_height,comp_grid_vt_test,presprite)

call action_sprite(presprite,"
var values=new Array();
var all_pos;
var max_cells=_root.max_cells;
var expected_ship;

function set_comp_ship()
{
    ship_nr=0;

    var row=0;var min_col=_root.grid_width-max_cells+1;
    all_pos=_root.grid_width*_root.grid_height;
    while(row<_root.grid_height)
    {
        var col=min_col;
        var off=max_cells;
        while(col<_root.grid_width)
        {
            comp_grid_hz_test[row][col]=off;
            off=off-1;
            col=col+1;
        }
        row=row+1;
    }
    var row=_root.grid_height-max_cells+1;
    var off=max_cells;
    while(row<_root.grid_height)
    {
        var col=0;
        while(col<_root.grid_width)
        {
            comp_grid_vt_test[row][col]=off;
            col=col+1;
        }
        off=off-1;
        row=row+1;
    }

    ai_init();
    comp_field_init();

    ship_at_comp();
}

function ship_at_comp()
{
    ship_string=_root.ships[ship_nr];
    if(ship_string==undefined)
    {
        return undefined;
    }
    ship_nr=ship_nr+1;

    var hz_vt=random(2);
    if(hz_vt==0)
    {
        ship_string=ship_string+'_horiz';
        grid_test=comp_grid_hz_test;
        attachMovie(ship_string,ship_string,getNextHighestDepth());
        expected_ship=this[ship_string];
        return undefined;
    }
    ship_string=ship_string+'_vert';
    grid_test=comp_grid_vt_test;
    attachMovie(ship_string,ship_string,getNextHighestDepth());
    expected_ship=this[ship_string];
}
function call_from_loaded_instance(loaded_instance)
{
    if(loaded_instance==expected_ship)
    {
        loaded_instance._visible=false;
        ship_comp_decision();
    }
}
var grid_test;
var ship_string;
function ship_comp_decision()
{
    var w=this[ship_string].width/_root.cell;
    var h=this[ship_string].height/_root.cell;

    var comp_pos=random(all_pos)+1;

    var nr=w;
    if(h>w)nr=h;
    var pos=0;
    while(pos<comp_pos)
    {
        pos=get_pos_in_grids(pos,grid_test,comp_pos,nr,w,h,this[ship_string]);
    }

    ship_at_comp();
}
//pos
function get_pos_in_grids(pos,grid_test,rand_pos,nr,w,h,inst)
{
    var row=0;
    while(row<_root.grid_height)
    {
        var col=0;
        while(col<_root.grid_width)
        {
            var can_be=1;
            var gr_pos=grid_test[row][col];
            if(gr_pos>0)if(gr_pos<=nr)can_be=0;
            if(can_be==1)
            {
                if(pos==rand_pos)
                {
                    if(w>h)
                    {
                        var row_max=row+1;var col_start=col-nr+1;var col_max=col+nr;
                        horiz_used(row,row_max,col_start,col,col_max);
                        var row_start=row-nr+1;row_max=row+1;col_max=col+nr;
                        vert_used(row_start,row,row_max,col,col_max);
                        place_comp_ship(row,col,inst);
                        return pos;
                    }
                    var row_max=row+nr;var col_start=col-nr+1;var col_max=col+1;
                    horiz_used(row,row_max,col_start,col,col_max);
                    var row_start=row-nr+1;row_max=row+nr;col_max=col+1;
                    vert_used(row_start,row,row_max,col,col_max);
                    place_comp_ship(row,col,inst);
                    return pos;
                }
                pos=pos+1;
            }
            col=col+1;
        }
        row=row+1;
    }
    return pos;
}
function horiz_used(row,row_max,col,ship_col,col_max)
{
    if(col<0)col=0;
    var c;
    while(row<row_max)
    {
        c=col;
        while(c<ship_col)
        {
            var newvalue=ship_col-c+1;
            set_nr_grid_test(comp_grid_hz_test,row,c,newvalue);
            c=c+1;
        }
        c=ship_col;
        while(c<col_max)
        {
            set_nr_grid_test(comp_grid_hz_test,row,c,1);
            c=c+1;
        }
        row=row+1;
    }
}
function vert_used(row,ship_row,row_max,col,col_max)
{
    if(row<0)row=0;
    var c;var off;
    while(row<ship_row)
    {
        c=col;off=ship_row-row+1;
        while(c<col_max)
        {
            set_nr_grid_test(comp_grid_vt_test,row,c,off);
            c=c+1;
        }
        row=row+1;
    }
    while(ship_row<row_max)
    {
        c=col;
        while(c<col_max)
        {
            set_nr_grid_test(comp_grid_vt_test,ship_row,c,1);
            c=c+1;
        }
        ship_row=ship_row+1;
    }
}
function set_nr_grid_test(grid_used,row,col,value)
{
    if(grid_used[row][col]==0){
        grid_used[row][col]=value;return undefined;}
    if(value<grid_used[row][col])grid_used[row][col]=value;
}
function place_comp_ship(row,col,inst)
{
    inst._x=col*_root.cell;
    inst._y=row*_root.cell;

    var r=0;var c;var start_col=col;
    while(r<inst.rows_nr)
    {
        c=0;col=start_col;
        while(c<inst.cols_nr)
        {
            _root.comp_grid[row][col]=ship_nr*inst.hz_vt;
            col=col+1;
            c=c+1;
        }
        row=row+1;
        r=r+1;
    }
}
")

#computer shoting the player

call action_sprite(presprite,"
var ai_fog=new Array();
var ai_hits=new Array();
var marker=100;
var comp_moved;
function ai_init()
{
    comp_moved=0;
    ai_fog.length=0;
    var ai_fog_total=_root.grid_width*_root.grid_height;
    var i=0;
    while(i<ai_fog_total){
        ai_fog.push(i);i=i+1;
    }
    ai_hits.length=0;
}

function ai_move()
{
    if(comp_moved==1)return undefined;
    if(ai_hits.length==0)
    {
        comp_hit(random(ai_fog.length));
        return undefined;
    }
    var pos;var shot_pos;
    var newpos=-1;
    while(newpos==-1)
    {
        pos=random(ai_hits.length);
        shot_pos=ai_hits[pos];
        newpos=around_the_target(shot_pos);
        if(newpos==-1)ai_hits.splice(pos,1);
    }

    var i=0;
    while(ai_fog[i]!=newpos)i=i+1;
    comp_hit(i);
}
function comp_hit(sorted_pos)
{
    var row;var col;var back;
    var grid_pos=ai_fog[sorted_pos];
    var w=_root.grid_width;

    row=grid_pos/w;row=int(row);back=row*w;
    col=grid_pos-back;
    var grid_object=_root.grid[row][col];
    _root.grid[row][col]=marker;
    var shot_used=0;

    if(grid_object!=0){
        ai_hits.push(grid_pos);

        var sufix='_horiz';
        if(grid_object<0){sufix='_vert';grid_object=grid_object*-1;}
        //1 goes 0 for ships[0]
        grid_object=grid_object-1;

        var ship_name=_root.ships[grid_object]+sufix;
        var inst=_root._grid1_map[ship_name];
        inst.size=inst.size-1;
        if(inst.size==0)
        {
            var ship_row=inst._y/_root.cell;var ship_col=inst._x/_root.cell;
            var r=0;var c;var start_col=ship_col;var g_pos;
            while(r<inst.rows_nr)
            {
                c=0;ship_col=start_col;
                while(c<inst.cols_nr)
                {
                    g_pos=ship_row*w+ship_col;
                    remove_from_array(ai_hits,g_pos);
                    _root.under_water(ship_row,ship_col,_root._grid1_map);
                    ship_col=ship_col+1;
                    c=c+1;
                }
                ship_row=ship_row+1;
                r=r+1;
            }
            _root._grid1_map.ship_nr=_root._grid1_map.ship_nr-1;
            if(_root._grid1_map.ship_nr==0)
            {
                comp_moved=1;
                var pen=_root.grid_width*_root.grid_height/2;
                _root.score=_root.score-pen;
                if(_root.score<1)_root.score=1;
                return undefined;
            }
            shot_used=1;
        }
        if(shot_used==0){_root.ship_shot(row,col,_root._grid1_map);shot_used=1;}
    }
    if(shot_used==0)_root.water_shot(row,col,_root._grid1_map);
    ai_fog.splice(sorted_pos,1);
}
function around_the_target(shot_pos)
{
    var row;var col;
    var w=_root.grid_width;var h=_root.grid_height;
    row=shot_pos/w;row=int(row);back=row*w;
    col=shot_pos-back;

    values.length=0;
    var target_pos;var c;var r;
    var gr=_root.grid;

    c=col-1;
    if(c>=0)if(gr[row][c]<marker)
    {
        target_pos=shot_pos-1;
        values.push(target_pos);
    }
    r=row-1;
    if(r>=0)if(gr[r][col]<marker)
    {
        target_pos=shot_pos-w;
        values.push(target_pos);
    }
    c=col+1;
    if(c<w)if(gr[row][c]<marker)
    {
        target_pos=shot_pos+1;
        values.push(target_pos);
    }
    r=row+1;
    if(r<h)if(gr[r][col]<marker)
    {
        target_pos=shot_pos+w;
        values.push(target_pos);
    }
    if(values.length==0)return -1;
    return values[random(values.length)];
}


function remove_from_array(array,value)
{
    var i=0;
    while(i<array.length)
    {
        if(array[i]==value)
        {
            array.splice(i,1);
            return undefined;
        }
        i=i+1;
    }
}
")

#player shoting the computer

call action_sprite(presprite,"
var sq_field;
var on_grid;
function comp_field_init()
{
    onMouseMove=mouse_comp_field;
    _root._text.message='Ready';
    on_grid=0;
}

function mouse_comp_field()
{
    var x=_root._xmouse-_x/_root.cell;x=int(x);
    var y=_root._ymouse-_y/_root.cell;var y=int(y);
    var is_in=comp_grid_test(x,y);
    if(on_grid==0)
    {
        if(is_in==0)return undefined;
        on_grid=1;
        _root.createEmptyMovieClip('square_field',_root.getNextHighestDepth());
        var square_field=_root.square_field;
        var cell=_root.cell;
        square_field.beginFill(0xffFF00,20);
        square_field.lineTo(cell,0);
        square_field.lineTo(cell,cell);
        square_field.lineTo(0,cell);
        square_field.lineTo(0,0);
        square_field.endFill();
        square_field.onPress=player_shot;
        _root.square_field._x=x*_root.cell+_x;
        _root.square_field._y=y*_root.cell+_y;
        return undefined;
    }
    if(is_in==0){on_grid=0;_root.square_field.removeMovieClip();return undefined;}
    _root.square_field._x=x*_root.cell+_x;
    _root.square_field._y=y*_root.cell+_y;
}
//0/1
function comp_grid_test(x,y)
{
    if(x<0)return 0;
    if(y<0)return 0;
    if(x>=_root.grid_width)return 0;
    if(y>=_root.grid_height)return 0;
    return 1;
}

function player_shot()
{
    var comp_turn=player_shot_action();
    if(comp_turn==1)ai_move();
}
//computer to move 0/1
function player_shot_action(){
    var row=_root.square_field._y-_y/_root.cell;row=int(row);
    var col=_root.square_field._x-_x/_root.cell;col=int(col);

    var grid_object=_root.comp_grid[row][col];
    var sufix='_horiz';
    if(grid_object<0){sufix='_vert';grid_object=grid_object*-1;}
    if(grid_object==marker)return 0;

    if(_root.score>1)_root.score=_root.score-1;
    _root._text.message=_root.score;

    _root.comp_grid[row][col]=marker;

    if(grid_object==0)
    {
        _root.water_shot(row,col,_root._grid2_map);
        return 1;
    }

    //1 goes 0 for ships[0]
    grid_object=grid_object-1;

    var ship_name=_root.ships[grid_object]+sufix;
    var inst=_root._grid2_map[ship_name];
    inst.size=inst.size-1;
    if(inst.size!=0)
    {
        _root.ship_shot(row,col,_root._grid2_map);
        return 1;
    }
    inst._visible=true;

    var ship_row=inst._y/_root.cell;var ship_col=inst._x/_root.cell;
    var r=0;var c;var start_col=ship_col;
    while(r<inst.rows_nr)
    {
        c=0;ship_col=start_col;
        while(c<inst.cols_nr)
        {
            _root.under_water(ship_row,ship_col,_root._grid2_map);
            ship_col=ship_col+1;
            c=c+1;
        }
        ship_row=ship_row+1;
        r=r+1;
    }

    ship_nr=ship_nr-1;
    if(ship_nr!=0)return 1;

    _root.onMouseMove=_root.void_function;
    _root.square_field.removeMovieClip();

    _root.submitform(_root.score);
    return 0;
}
")
