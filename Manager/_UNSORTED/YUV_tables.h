#pragma once

const int RGB_db_term_one[256] = {-174645, -163002, -151359, -139716, -128073, -116430, -104787, -93144, -81501, -69858, -58215, -46572, -34929, -23286, -11643, 0, 11643, 23286, 34929, 46572, 58215, 69858, 81501, 93144, 104787, 116430, 128073, 139716, 151359, 163002, 174645, 186288, 197931, 209574, 221217, 232860, 244503, 256146, 267789, 279432, 291075, 302718, 314361, 326004, 337647, 349290, 360933, 372576, 384219, 395862, 407505, 419148, 430791, 442434, 454077, 465720, 477363, 489006, 500649, 512292, 523935, 535578, 547221, 558864, 570507, 582150, 593793, 605436, 617079, 628722, 640365, 652008, 663651, 675294, 686937, 698580, 710223, 721866, 733509, 745152, 756795, 768438, 780081, 791724, 803367, 815010, 826653, 838296, 849939, 861582, 873225, 884868, 896511, 908154, 919797, 931440, 943083, 954726, 966369, 978012, 989655, 1001298, 1012941, 1024584, 1036227, 1047870, 1059513, 1071156, 1082799, 1094442, 1106085, 1117728, 1129371, 1141014, 1152657, 1164300, 1175943, 1187586, 1199229, 1210872, 1222515, 1234158, 1245801, 1257444, 1269087, 1280730, 1292373, 1304016, 1315659, 1327302, 1338945, 1350588, 1362231, 1373874, 1385517, 1397160, 1408803, 1420446, 1432089, 1443732, 1455375, 1467018, 1478661, 1490304, 1501947, 1513590, 1525233, 1536876, 1548519, 1560162, 1571805, 1583448, 1595091, 1606734, 1618377, 1630020, 1641663, 1653306, 1664949, 1676592, 1688235, 1699878, 1711521, 1723164, 1734807, 1746450, 1758093, 1769736, 1781379, 1793022, 1804665, 1816308, 1827951, 1839594, 1851237, 1862880, 1874523, 1886166, 1897809, 1909452, 1921095, 1932738, 1944381, 1956024, 1967667, 1979310, 1990953, 2002596, 2014239, 2025882, 2037525, 2049168, 2060811, 2072454, 2084097, 2095740, 2107383, 2119026, 2130669, 2142312, 2153955, 2165598, 2177241, 2188884, 2200527, 2212170, 2223813, 2235456, 2247099, 2258742, 2270385, 2282028, 2293671, 2305314, 2316957, 2328600, 2340243, 2351886, 2363529, 2375172, 2386815, 2398458, 2410101, 2421744, 2433387, 2445030, 2456673, 2468316, 2479959, 2491602, 2503245, 2514888, 2526531, 2538174, 2549817, 2561460, 2573103, 2584746, 2596389, 2608032, 2619675, 2631318, 2642961, 2654604, 2666247, 2677890, 2689533, 2701176, 2712819, 2724462, 2736105, 2747748, 2759391, 2771034, 2782677, 2794320 };

const int RED_db_term_two[256]		= {-2026920, -2010960, -1995000, -1979040, -1963080, -1947120, -1931160, -1915200, -1899240, -1883280, -1867320, -1851360, -1835400, -1819440, -1803480, -1787520, -1771560, -1755600, -1739640, -1723680, -1707720, -1691760, -1675800, -1659840, -1643880, -1627920, -1611960, -1596000, -1580040, -1564080, -1548120, -1532160, -1516200, -1500240, -1484280, -1468320, -1452360, -1436400, -1420440, -1404480, -1388520, -1372560, -1356600, -1340640, -1324680, -1308720, -1292760, -1276800, -1260840, -1244880, -1228920, -1212960, -1197000, -1181040, -1165080, -1149120, -1133160, -1117200, -1101240, -1085280, -1069320, -1053360, -1037400, -1021440, -1005480, -989520, -973560, -957600, -941640, -925680, -909720, -893760, -877800, -861840, -845880, -829920, -813960, -798000, -782040, -766080, -750120, -734160, -718200, -702240, -686280, -670320, -654360, -638400, -622440, -606480, -590520, -574560, -558600, -542640, -526680, -510720, -494760, -478800, -462840, -446880, -430920, -414960, -399000, -383040, -367080, -351120, -335160, -319200, -303240, -287280, -271320, -255360, -239400, -223440, -207480, -191520, -175560, -159600, -143640, -127680, -111720, -95760, -79800, -63840, -47880, -31920, -15960, 0, 15960, 31920, 47880, 63840, 79800, 95760, 111720, 127680, 143640, 159600, 175560, 191520, 207480, 223440, 239400, 255360, 271320, 287280, 303240, 319200, 335160, 351120, 367080, 383040, 399000, 414960, 430920, 446880, 462840, 478800, 494760, 510720, 526680, 542640, 558600, 574560, 590520, 606480, 622440, 638400, 654360, 670320, 686280, 702240, 718200, 734160, 750120, 766080, 782040, 798000, 813960, 829920, 845880, 861840, 877800, 893760, 909720, 925680, 941640, 957600, 973560, 989520, 1005480, 1021440, 1037400, 1053360, 1069320, 1085280, 1101240, 1117200, 1133160, 1149120, 1165080, 1181040, 1197000, 1212960, 1228920, 1244880, 1260840, 1276800, 1292760, 1308720, 1324680, 1340640, 1356600, 1372560, 1388520, 1404480, 1420440, 1436400, 1452360, 1468320, 1484280, 1500240, 1516200, 1532160, 1548120, 1564080, 1580040, 1596000, 1611960, 1627920, 1643880, 1659840, 1675800, 1691760, 1707720, 1723680, 1739640, 1755600, 1771560, 1787520, 1803480, 1819440, 1835400, 1851360, 1867320, 1883280, 1899240, 1915200, 1931160, 1947120, 1963080, 1979040, 1995000, 2010960, 2026920, 2042880 };

const int GREEN_db_term_two[256]	= {-497586, -493668, -489750, -485832, -481914, -477996, -474078, -470160, -466242, -462324, -458406, -454488, -450570, -446652, -442734, -438816, -434898, -430980, -427062, -423144, -419226, -415308, -411390, -407472, -403554, -399636, -395718, -391800, -387882, -383964, -380046, -376128, -372210, -368292, -364374, -360456, -356538, -352620, -348702, -344784, -340866, -336948, -333030, -329112, -325194, -321276, -317358, -313440, -309522, -305604, -301686, -297768, -293850, -289932, -286014, -282096, -278178, -274260, -270342, -266424, -262506, -258588, -254670, -250752, -246834, -242916, -238998, -235080, -231162, -227244, -223326, -219408, -215490, -211572, -207654, -203736, -199818, -195900, -191982, -188064, -184146, -180228, -176310, -172392, -168474, -164556, -160638, -156720, -152802, -148884, -144966, -141048, -137130, -133212, -129294, -125376, -121458, -117540, -113622, -109704, -105786, -101868, -97950, -94032, -90114, -86196, -82278, -78360, -74442, -70524, -66606, -62688, -58770, -54852, -50934, -47016, -43098, -39180, -35262, -31344, -27426, -23508, -19590, -15672, -11754, -7836, -3918, 0, 3918, 7836, 11754, 15672, 19590, 23508, 27426, 31344, 35262, 39180, 43098, 47016, 50934, 54852, 58770, 62688, 66606, 70524, 74442, 78360, 82278, 86196, 90114, 94032, 97950, 101868, 105786, 109704, 113622, 117540, 121458, 125376, 129294, 133212, 137130, 141048, 144966, 148884, 152802, 156720, 160638, 164556, 168474, 172392, 176310, 180228, 184146, 188064, 191982, 195900, 199818, 203736, 207654, 211572, 215490, 219408, 223326, 227244, 231162, 235080, 238998, 242916, 246834, 250752, 254670, 258588, 262506, 266424, 270342, 274260, 278178, 282096, 286014, 289932, 293850, 297768, 301686, 305604, 309522, 313440, 317358, 321276, 325194, 329112, 333030, 336948, 340866, 344784, 348702, 352620, 356538, 360456, 364374, 368292, 372210, 376128, 380046, 383964, 387882, 391800, 395718, 399636, 403554, 407472, 411390, 415308, 419226, 423144, 427062, 430980, 434898, 438816, 442734, 446652, 450570, 454488, 458406, 462324, 466242, 470160, 474078, 477996, 481914, 485832, 489750, 493668, 497586, 501504};
const int GREEN_db_term_three[256]	= {-1032383, -1024254, -1016125, -1007996, -999867, -991738, -983609, -975480, -967351, -959222, -951093, -942964, -934835, -926706, -918577, -910448, -902319, -894190, -886061, -877932, -869803, -861674, -853545, -845416, -837287, -829158, -821029, -812900, -804771, -796642, -788513, -780384, -772255, -764126, -755997, -747868, -739739, -731610, -723481, -715352, -707223, -699094, -690965, -682836, -674707, -666578, -658449, -650320, -642191, -634062, -625933, -617804, -609675, -601546, -593417, -585288, -577159, -569030, -560901, -552772, -544643, -536514, -528385, -520256, -512127, -503998, -495869, -487740, -479611, -471482, -463353, -455224, -447095, -438966, -430837, -422708, -414579, -406450, -398321, -390192, -382063, -373934, -365805, -357676, -349547, -341418, -333289, -325160, -317031, -308902, -300773, -292644, -284515, -276386, -268257, -260128, -251999, -243870, -235741, -227612, -219483, -211354, -203225, -195096, -186967, -178838, -170709, -162580, -154451, -146322, -138193, -130064, -121935, -113806, -105677, -97548, -89419, -81290, -73161, -65032, -56903, -48774, -40645, -32516, -24387, -16258, -8129, 0, 8129, 16258, 24387, 32516, 40645, 48774, 56903, 65032, 73161, 81290, 89419, 97548, 105677, 113806, 121935, 130064, 138193, 146322, 154451, 162580, 170709, 178838, 186967, 195096, 203225, 211354, 219483, 227612, 235741, 243870, 251999, 260128, 268257, 276386, 284515, 292644, 300773, 308902, 317031, 325160, 333289, 341418, 349547, 357676, 365805, 373934, 382063, 390192, 398321, 406450, 414579, 422708, 430837, 438966, 447095, 455224, 463353, 471482, 479611, 487740, 495869, 503998, 512127, 520256, 528385, 536514, 544643, 552772, 560901, 569030, 577159, 585288, 593417, 601546, 609675, 617804, 625933, 634062, 642191, 650320, 658449, 666578, 674707, 682836, 690965, 699094, 707223, 715352, 723481, 731610, 739739, 747868, 755997, 764126, 772255, 780384, 788513, 796642, 804771, 812900, 821029, 829158, 837287, 845416, 853545, 861674, 869803, 877932, 886061, 894190, 902319, 910448, 918577, 926706, 934835, 942964, 951093, 959222, 967351, 975480, 983609, 991738, 999867, 1007996, 1016125, 1024254, 1032383, 1040512};
	
const int BLUE_db_term_two[256] = {-2561844, -2541672, -2521500, -2501328, -2481156, -2460984, -2440812, -2420640, -2400468, -2380296, -2360124, -2339952, -2319780, -2299608, -2279436, -2259264, -2239092, -2218920, -2198748, -2178576, -2158404, -2138232, -2118060, -2097888, -2077716, -2057544, -2037372, -2017200, -1997028, -1976856, -1956684, -1936512, -1916340, -1896168, -1875996, -1855824, -1835652, -1815480, -1795308, -1775136, -1754964, -1734792, -1714620, -1694448, -1674276, -1654104, -1633932, -1613760, -1593588, -1573416, -1553244, -1533072, -1512900, -1492728, -1472556, -1452384, -1432212, -1412040, -1391868, -1371696, -1351524, -1331352, -1311180, -1291008, -1270836, -1250664, -1230492, -1210320, -1190148, -1169976, -1149804, -1129632, -1109460, -1089288, -1069116, -1048944, -1028772, -1008600, -988428, -968256, -948084, -927912, -907740, -887568, -867396, -847224, -827052, -806880, -786708, -766536, -746364, -726192, -706020, -685848, -665676, -645504, -625332, -605160, -584988, -564816, -544644, -524472, -504300, -484128, -463956, -443784, -423612, -403440, -383268, -363096, -342924, -322752, -302580, -282408, -262236, -242064, -221892, -201720, -181548, -161376, -141204, -121032, -100860, -80688, -60516, -40344, -20172, 0, 20172, 40344, 60516, 80688, 100860, 121032, 141204, 161376, 181548, 201720, 221892, 242064, 262236, 282408, 302580, 322752, 342924, 363096, 383268, 403440, 423612, 443784, 463956, 484128, 504300, 524472, 544644, 564816, 584988, 605160, 625332, 645504, 665676, 685848, 706020, 726192, 746364, 766536, 786708, 806880, 827052, 847224, 867396, 887568, 907740, 927912, 948084, 968256, 988428, 1008600, 1028772, 1048944, 1069116, 1089288, 1109460, 1129632, 1149804, 1169976, 1190148, 1210320, 1230492, 1250664, 1270836, 1291008, 1311180, 1331352, 1351524, 1371696, 1391868, 1412040, 1432212, 1452384, 1472556, 1492728, 1512900, 1533072, 1553244, 1573416, 1593588, 1613760, 1633932, 1654104, 1674276, 1694448, 1714620, 1734792, 1754964, 1775136, 1795308, 1815480, 1835652, 1855824, 1875996, 1896168, 1916340, 1936512, 1956684, 1976856, 1997028, 2017200, 2037372, 2057544, 2077716, 2097888, 2118060, 2138232, 2158404, 2178576, 2198748, 2218920, 2239092, 2259264, 2279436, 2299608, 2319780, 2339952, 2360124, 2380296, 2400468, 2420640, 2440812, 2460984, 2481156, 2501328, 2521500, 2541672, 2561844, 2582016};