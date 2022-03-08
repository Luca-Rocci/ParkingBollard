use <MCAD/boxes.scad>
use <MCAD/servos.scad>
use <MCAD/regular_shapes.scad>
use <MCAD/nuts_and_bolts.scad>
use <MCAD/materials.scad>

// Materiali e colori
PvcBianco = "#FFFFFF";
BandaRossa = "#FF2525";
FerroZincato = "#E5E5E5";
PlasticaNera = "#353535";
FerroScuro = "#757575";

module paletto(r,h,n) { 
    union() {
        hb = h/n;
        for (i = [0 : n-1]) {
            let (z = i*hb, c = i%2==1 ? BandaRossa : PvcBianco) {
                translate([0,0,z])
                    color(c)
                        cylinder_tube(hb, r, 1);
            }
        }
    }
}



module piastra(x,y,z) {
    color(FerroScuro)
    union() {
        translate([45,0,0])
            rotate([90,0,90]) 
                difference() {
                    piasta_verticale([y,z,1],10);
                    translate([19,10,-1])
                        cube([42,22,3]);
                }
        translate([95,0,0])
            rotate([90,0,90]) 
                difference() {
                    piasta_verticale([y,z,1],10);
                    translate([50,21,-1])
                        cylinder(6,3,3,true);
                }
        difference() {
            roundedCube([x,y,1], 5, true, false);
            translate([12,12,-1])
                cylinder(6,3,3,true);
            translate([x-12,y-12,-1])
                cylinder(6,3,3,true);
            translate([12,y-12,-1])
                cylinder(6,3,3,true); 
            translate([x-12,12,-1])
                cylinder(6,3,3,true); 
            translate([12,y/2,-1])
                cylinder(6,6,6,true); 
            translate([x-12,y/2,-1])
                cylinder(6,6,6,true);  
        }

    }
}

module piasta_verticale(s,r) {
    hull() {
        cube([s[0],s[1]/3,s[2]]);
        translate([(s[0]/5)+r,s[1]-r]) 
            cylinder(r=r, h=s[2]);
        translate([s[0]-(s[0]/5)-r,s[1]-r]) 
            cylinder(r=r, h=s[2]);
    }
}

module curva(r,g) {
    color(PvcBianco)
        rotate_extrude(angle = g, convexity = 2)
            translate([120, 0, 0])
                ring(r*2, 1);
}

module paletto_singolo(h,n,r) {
    translate([0,0,21]) rotate([-r,0,0]) translate([0,0,-9]) paletto(16,h,n);

    translate([-77,-50,0]) piastra(120,80,40);

    color(PlasticaNera) futabas3003([-57, -30, 31],[0, 90, 0]);

    translate([20,0,21])
        rotate([90,0,-90]) 
            color(FerroZincato) 
                boltHole(5, length= 10, proj=-1);
}

module barriera(h,l,n) {
    hp = h - 120;
    hb = (l-240)/(n-1);
    paletto_singolo(hp,n,0);
    translate([120,0,hp + 12]) rotate([90,0,180]) curva(16,90);
    translate([l,0,0]) paletto_singolo(hp,n,0);
    translate([l-120,0,hp + 12]) rotate([90,0,0]) curva(16,90);
    union() {
        for (i = [0 : n-2]) {
            let (x = i*hb, c = i%2==1 ? PvcBianco : BandaRossa) {
                translate([120+x,0,h+12])
                    color(c) rotate([90,0,90])
                        cylinder_tube(hb, 16, 1);
            }
        }
    }
}

$fn=100;
//if($t<=0.5) {
//paletto_singolo(800,8,180*$t);
//} else {
//    paletto_singolo(800,8,180-(180*$t));
//}

//translate([200,0,0])  paletto_singolo(800,8,45);

//translate([400,0,0])  paletto_singolo(800,8,90);

//translate([600,0,0]) 
    barriera(700,700,6);