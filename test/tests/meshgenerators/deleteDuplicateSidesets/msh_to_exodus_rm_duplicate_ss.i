# This is a MOOSE input file to remove duplicate elements from sidesets created by Andy's script
# Run it using:

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = rkp_forge_10zone_10m.msh
  []
  [rmDuplicateSS]
    input = fmg
    type = RemoveDuplicateFacesFromSidesets
  []
  [fracture]
    input = rmDuplicateSS
    type = LowerDBlockFromSidesetGenerator
    new_block_id = 11
    new_block_name = 'fracture'
    sidesets = 'disk1 disk2 disk3 disk4 disk5 disk6 disk7 disk8 disk9
    disk10 disk11 disk12 disk13 disk14 disk15 disk16 disk17 disk18 disk19
    disk20 disk21 disk22 disk23 disk24 disk25 disk26 disk27 disk28 disk29
    disk30 disk31 disk32 disk33 disk34 disk35 disk36 disk37 disk38 disk39
    disk40 disk41 disk42 disk43 disk44 disk45 disk46 disk47 disk48 disk49
    disk50 disk51 disk52 disk53 disk54 disk55 disk56 disk57 disk58 disk59
    disk60 disk61 disk62 disk63 disk64 disk65 disk66 disk67 disk68 disk69
    disk70 disk71 disk72 disk73 disk74 disk75 disk76 disk77 disk78 disk79
    disk80 disk81 disk82 disk83 disk84 disk85 disk86 disk87 disk88 disk89
    disk90 disk91 disk92 disk93 disk94 disk95 disk96 disk97 disk98 disk99
    disk100 disk101 disk102 disk103 disk104 disk105 disk106 disk107 disk108 disk109
    disk110 disk111 disk112 disk113 disk114 disk115 disk116 disk117 disk118 disk119
    disk120'
  []
[]
