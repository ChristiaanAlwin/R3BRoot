Double_t DefineParticleMass(Int_t const PDG)
{
    // Defines the particle mass in [GeV] given the PDG code.
    // definitions are taken from the list defined here:
    
    Double_t Mass = -1.0;
    
    // Check our list:
    if (PDG==2212) {Mass = 0.938272046;}	            // Proton mass
    if (PDG==2112) {Mass = 0.939565379;}              	    // Neutron mass
    if (PDG==211) {Mass = 0.13957018;}	                    // Pion mass
    if (PDG==11) {Mass = 0.00051099891013;}	            // Elektron mass
    if (PDG==1000200480) {Mass = 44.6573;}                 // Ca48 mass
    if (PDG==1000020040) {Mass = 3.72737950844;}           // Alpha mass
    if (PDG==1000922380) {Mass = 221.74;}                  // U238 mass
    if (PDG==1000080240) {Mass = 22.375;}                  // O24 mass
    if (PDG==1000090290) {Mass = 26.961;}                  // F29 mass
    if (PDG==1000080160) {Mass = 14.899;}                  // O16 mass
    if (PDG==1000501120) {Mass = 110.58;}                  // Sn-112 mass
    if (PDG==1000501240) {Mass = 115.42;}                  // Sn-124 mass
    
    // return the mass:
    return Mass;
}