#!/usr/bin/env python
import sys
runNo = sys.argv[1]
evenNo = sys.argv[2]
#raw_input("Enter the run number: ")
#eveNo = $2
#raw_input("Enter the event number: ")
filename = './REPORT_OUTPUT/SHMS/PRODUCTION/replay_shms_production_%s_%s.report'% (runNo, evenNo)

f = open(filename)
fout = open('output.txt','w')

for line in f:
    data = line.split(':')
    if ('Run #' in data[0])or ('3_of_4 EFF' in data[0])  or ('BCM1 Beam Cut Current'in data[0]) or ('BCM2 Beam Cut Current'in data[0]) or('BCM4a Beam Cut Current' in data[0]) or ('SING FID' in data[0]and 'HADRON' not in data[0])or('Plane 1' in data[0]) or('Plane 2' in data[0])or('Plane 3'in data[0])or ('Plane 4'in data[0]):
      

        # print (data[0]+ ': '+ data[1])
      
        fout.write(data[0]+ ': '+ data[1])

    
   
  	
	
    data2 = line.split('=')
    if ('Ps' in data2[0] and 'factor' in data2[0] and 'Ps4' not in data2[0] and 'Ps5' not in data2[0] and 'Ps6' not in data2[0]):
        line = data2[0]
        line = line.replace('Ps1_factor','Pre-Scale SHMS 3/4')
        line = line.replace('Ps2_factor','Pre-Scale SHMS EL-Real')
        line = line.replace('Ps3_factor','Pre-Scale SHMS EL-clean')
        
      

    # print (line + ': '+ data2[1])
      
        fout.write(line + ':'+ data2[1])
    
fout.close()
f.close()
