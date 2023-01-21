function f1 pi1 pa2
def
enddef
return pi1
end

function f2 pa1 pa2
def
var vi1
vet va2 size ci5
var vi3
vet va4 size ci5
var vi5
enddef



vi1 = ci1
vi1 = vi3
vi1 = pa2

vi5 = ci1 / vi2 
vi5 = pi1 / ci3
vi5 = vi1 / pi2 


vi1 = call f1 ci1 
vi3 = call f2 vi1 va3
vi5 = call f3 ci-5 pi1 va3
vi5 = call f4 pa2 pa3 pi1

if vi3
vi5 = call f4 pa2 pa3 pi1
endif

if ci5
return vi5
endif

if pi2
return ci3
endif


get va2 index ci2 to pi2
get va4 index ci4 to vi1

set va2 index ci2 with ci2
set pa2 index ci1 with vi1
set pa2 index ci0 with pi1


return vi3
end
