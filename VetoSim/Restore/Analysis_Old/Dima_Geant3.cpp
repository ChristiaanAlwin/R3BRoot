Dear Christiaan,

after debugging your error the whole day, I can tell that it is not related to R3BRoot or Geant3. It is Fluka part, which can not handle hadronic interactions of your "exotic" isotopes. The only way to overcome it, is to set HADR option to 1 in gconfig/SetCuts.C. BUT then you will not get your neutrons correct. So it is not an option.

Sorry, I can not help your with this issue.

Best regards,
Dima