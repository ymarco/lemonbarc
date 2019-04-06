#./lemonbarc | lemonbar -f "Ubuntu:size=14" -u 3 -B "#90202020" -U "#00000000"
#
f1='Ubuntu:size=14'
f2='Terminal:size=14' 
f3='FontAwesome5Free:size=13'  
f4='FontAwesome5FreeSolid:size=13' 
f5='FontAwesome5Brands:size=13'

bg="#a9202020"

./fullscreenhide.sh &
./lemonbarc | lemonbar -f "$f1" -f "$f2" -f "$f3" -f "$f4" -f "$f5" -u 2 -B $bg -U $bg
