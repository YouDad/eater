// Copyright 2020 <luwh0708@thundersoft.com>
#include "modules/algorithm.h"
#include "modules/draw.h"
#include "utils/assert.h"

#define test(map, size, id, move, is_fire) {\
	class server_data data((char *)map, size, id);\
	auto result = algorithm(data);\
	if (result.first != move || result.second != is_fire) { \
		for (int i = 0; i < size + 1; i++) { \
			printf("\n"); \
		} \
		draw((char *)map, size, id); \
		printf("%d, %d\n", result.first, result.second); \
	} \
	assert(result.first == move);\
	assert(result.second == is_fire);\
}

int main() {
	/*
	  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0   score:
	  0 0 0 口口4 0 口0 1 0 0 0 0 0 0 0 0 1 0 0 0 0 口0   player 0: 0
	  0 0 0 0 ☣ 0 0 口0 0 0 0 0 0 0 0 0 0 0 0 口0 ☣ 4 0   player 1: 240
	  0 0 0 0 0 1 口3 0 0 0 0 口2 0 0 0 0 0 0 0 0 0 0 0   player 2: 143
	  0 0 0 0 0 1 0 0 0 口0 0 1 3 0 0 0 0 口4 0 0 0 口0   player 3: 67
	  0 0 0 0 0 口0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0   player 4: 106
	  0 0 0 0 0 2 0 口0 ☣ 0 0 0 口0 0 口0 口5 1 0 0 0 0   player 5: 134
	  0 0 口0 口口0 0 0 0 0 0 0 0 0 0 0 0 2 2 口1 口0 0   player 6: 242
	  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 4 口2 2 2 0 0   player 7: 261
	  0 0 口口0 0 0 0 0 0 0 0 0 口0 0 0 0 口口3 1 口0 0
	  0 0 1 0 0 0 0 0 0 口0 0 口☣ 0 0 口0 0 0 0 0 0 0 0
	  0 0 0 0 0 口0 0 0 0 0 0 0 0 >70 口0 0 0 0 0 0 0 0
	  0 2 口口0 0 0 0 口0 0 0 0 0 ☣ 0 5 0 0 1 0 3 1 口1
	  0 0 0 口口0 0 0 口0 ☣ 0 0 0 0 0 口0 0 2 0 口0 0 0
	  0 1 0 口1 0 0 0 0 0 0 0 口0 0 0 0 口口口0 3 0 0 0
	  0 口0 0 0 0 0 0 0 0 0 0 0 0 口口0 >60 0 0 3 0 0 0
	  0 口0 1 3 口0 0 0 0 口口0 0 口口0 0 1 5 0 口0 0 0
	  0 1 0 2 口0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	  0 0 0 2 口0 0 0 0 0 口0 0 0 口0 0 口1 口0 0 0 0 0
	  0 0 0 口0 0 0 0 口0 0 0 0 0 0 0 0 口>10 0 0 口0 0
	  0 0 0 0 口0 0 0 3 0 0 0 0 口0 0 0 0 口0 ☣ 0 0 0 0
	  0 口0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	  0 0 口0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 口0 0
	  0 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 口1 0 1 0 0 0 0
	  0 0 0 0 0 0 0 口0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	round (312/370)
	 */
	test("[MAP OJ "
		"0000000000000000000000000"
		"0009940901000000001000090"
		"0000G00900000000000090G40"
		"0000019300009200000000000"
		"0000010009001300009400090"
		"0000090200000000000000000"
		"000002090G000900909510000"
		"0090990000000000002291900"
		"0000000000000000004922200"
		"0099000000000900009931900"
		"0010000009009G00900000000"
		"00000900000000d0900000000"
		"02990000900000G0500103191"
		"0009900090G00000900209000"
		"0109100000009000099903000"
		"09000000000000990d0003000"
		"0901390000990099001509000"
		"0102900000000000000000000"
		"0002900000900090091900000"
		"000900009000000009d000900"
		"00009000300009000090G0000"
		"0900000000000000000000000"
		"0090000000000000000000900"
		"0020000000000000091010000"
		"0000000900000000000000000]"
		"[LOCATION OJ -1 493 -1 -1 -1 -1 392 289]"
		"[SCORE OJ 0 240 143 67 106 134 242 261]",
		25, 7, move_op_down, true);

	/* round 75:
	  0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 5 3 5 2 5 2 3 4 5 2   score:
	  2 3 0 4 0 2 4 0 1 1 1 0 0 0 0 4 1 5 5 2 2 4 5 口2   player 0: 35
	  1 0 0 口0 0 0 0 0 0 0 口0 0 2 2 2 口1 4 2 2 口3 1   player 1: 37
	  2 v7口0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 1 3 2 3 1 口3   player 2: 67
	  4 ☣ 3 0 3 0 0 0 0 口口0 >3<10 3 0 2 3 1 2 1 口1 2   player 3: 36
	  2 3 4 0 口0 1 0 口0 0 v65 4 3 口0 2 1 2 2 口5 1 1   player 4: 8
	  4 口5 0 5 0 1 0 1 0 5 1 4 4 1 2 0 2 4 4 5 3 2 5 4   player 5: 22
	  0 0 0 0 口0 1 0 3 0 口口2 1 0 0 0 0 2 3 3 3 5 口3   player 6: 71
	  0 0 0 0 口0 2 0 0 0 口口3 口1 0 0 0 1 2 口2 4 4 4   player 7: 49
	  0 5 口0 3 0 0 0 4 1 1 3 口3 4 口口4 2 1 5 5 3 2 2   player 8: 14
	  0 3 口0 口2 0 0 0 0 0 0 0 0 口1 口1 4 5 2 4 口5 1
	  0 0 0 0 0 1 5 v4口1 1 0 0 0 口4 1 5 5 5 口口3 口4
	  0 0 0 0 4 口口▼ 3 口5 2 口2 3 2 4 4 1 4 3 5 口口3
	  0 0 0 0 3 3 5 >55 4 4 3 口口3 2 2 4 2 2 5 5 3 3 4
	  1 4 0 口口5 4 0 5 口3 1 5 5 0 0 0 1 3 5 3 4 3 2 3
	  5 口0 1 口2 3 0 4 ☣ 4 1 1 4 3 5 3 5 3 4 1 4 口1 2
	  1 4 0 4 口3 2 0 口3 1 3 1 3 2 4 5 5 3 1 2 2 5 4 5
	  1 2 0 ☣ 3 口1 0 0 口1 3 1 1 1 1 4 5 口3 口2 口3 2
	  1 3 v24 2 2 5 5 0 2 4 1 口1 5 2 2 4 5 2 口3 2 2 4
	  4 2 4 口1 1 5 口0 1 3 2 5 ☣ ☣ 1 2 口5 1 2 5 5 4 1
	  3 3 2 口口5 1 5 0 3 1 2 2 3 4 2 口1 1 2 3 2 2 2 5
	  2 4 1 3 5 4 1 2 0 5 5 1 2 4 2 3 1 3 5 口2 口4 2 1
	  5 5 2 5 口2 0 0 0 1 口2 3 3 口2 4 2 口2 1 5 2 4 1
	  3 3 1 3 0 0 0 1 4 3 5 4 4 口3 2 1 3 2 口1 口1 2 4
	  1 4 2 4 口4 2 1 口5 4 3 1 3 5 3 2 2 4 1 3 1 3 3 4 */
	test("[MAP Rh "
		"0000000000000505352523452"
		"2304024011100004155224592"
		"1009000000090022291422931"
		"2s90000000000000021323193"
		"4G3030000990da03023121912"
		"23409010900s5439021229511"
		"4950501010514412024453254"
		"0000901030992100002333593"
		"0000902000993910001292444"
		"0590300041139349942155322"
		"0390920000000091914524951"
		"0000015s91100094155599394"
		"0000499v39529232441435993"
		"0000335d54439932242255334"
		"1409954059315500013534323"
		"590192304G411435353414912"
		"1404932093131324553122545"
		"120G391009131111459392932"
		"13s4225502419152245293224"
		"4249115901325GG1295125541"
		"3329951503122342911232225"
		"2413541205512423135929421"
		"5525920001923392429215241"
		"3313000143544932132919124"
		"1424942195431353224131334]"
		"[LOCATION Rh -1 113 452 112 282 332 136 76 -1]"
		"[SCORE Rh 35 37 67 36 8 22 71 49 14]",
		25, 7, move_op_stay, true);

	test("[MAP qN "
		"2522521012441202323345552"
		"5541244022a00003233595514"
		"3323342015291004239212553"
		"5111493045345954542114912"
		"4291113091342121394292514"
		"5999445022941149221591412"
		"1959252021545111535352995"
		"00G000da41100002949933333"
		"12199143a0009453593531191"
		"4415339593255394535434324"
		"3349349331595352214131544"
		"4439113555522541292953324"
		"3224535915139412145221543"
		"1411193299934321241241925"
		"2313414000000a21242359312"
		"2599995945152243154141492"
		"5923141449232429534514995"
		"4110041545144115244311352"
		"2a00019539923122491225333"
		"2331031153340002354249331"
		"5134444511350334943392433"
		"152532991a000193351991441"
		"4992314592924399441991532"
		"3221455132341423599191321"
		"5432559315233214544355241]"
		"[LOCATION qN 451 35 181 534 208 182 363]"
		"[SCORE qN 31 26 17 35 33 19 27]",
		25, 5, move_op_stay, true);

	test("[MAP @R "
		"0000000014143443224211152"
		"3124013211000000951515534"
		"3199000000011440200000115"
		"1135001003352910202490344"
		"0091000101294390103930395"
		"0092012901331000200000245"
		"00190000013130009w5042222"
		"0094445113493100000012314"
		"0000321395499190909239491"
		"0000912131000000000092342"
		"0990000331052G00000025951"
		"0000000959039239200000000"
		"0000000120000001940910000"
		"a009900094954900390139300"
		"0090000012131400000944300"
		"0000090000999990120393900"
		"0090090910932240000000300"
		"0000920000000929350000300"
		"0091590111090000000000900"
		"0913000200092524290941500"
		"0000099100095114900000000"
		"1000000000024399300359900"
		"3030000000091131100000000"
		"5090000023019313290004000"
		"4000923422000000000002000]"
		"[LOCATION @R -1 -1 -1 167 -1 325 -1]"
		"[SCORE @R 0 73 8 251 79 266 15]",
		25, 3, move_op_right, false);

	test("[MAP ii "
		"0000000000000000001513132"
		"2544113103492141905352914"
		"1959534205951935202452534"
		"2392023504954393204234541"
		"1129039401431991905154992"
		"4921Gd5100005359204145132"
		"1331009424205953505499323"
		"0000005995302211305955932"
		"53913134190d0000002513955"
		"243200013934da00001442453"
		"4941010515239435504133391"
		"2121050025943311302321154"
		"5423092042213314103153913"
		"3391033033993115101292992"
		"55500d5092954349909353942"
		"3220019911919392109251142"
		"4410000429530w13301331222"
		"4213901450000494101422421"
		"1351504410333194304913914"
		"4139000000455900005223232"
		"3323003212999224453449242"
		"1422034245291294151293441"
		"4321041533539493541595595"
		"2421091411421121253232355"
		"3541915293443525213244255]"
		"[LOCATION ii 355 211 237 130 -1 413 -1 238]"
		"[SCORE ii 68 45 6 13 33 32 1 12]",
		25, 0, move_op_right, false);

	test("[MAP L6 "
		"00000d300000d254421513132"
		"2544113143492141945352914"
		"1959534235951935242452534"
		"2392023534954393214234541"
		"1129039421431991955154992"
		"4921035122145359254145132"
		"1331s59424255953545499323"
		"000d5359G5312211345955932"
		"5391313419445224412513955"
		"2432422139345352351442453"
		"4941115515239435544133391"
		"212155a025943311342321154"
		"5423492042213314113153913"
		"3391533033993115131292992"
		"5554215092954349959353942"
		"3225219911919392119251142"
		"4415545429534413321331222"
		"4213941455131494151422421"
		"13515344153331943w4913914"
		"413900000w455900005223232"
		"33230d3212999224453449242"
		"1422034245291294151293441"
		"4321041533539493541595595"
		"2421091411421121253232355"
		"3541915293443525213244255]"
		"[LOCATION L6 281 5 467 178 505 484 154 12]"
		"[SCORE L6 25 22 22 19 20 24 15 24]",
		25, 0, move_op_left, false);

	test("[MAP eS "
		"3131521453255351343513513"
		"4415332445924459511421422"
		"5243424225493345231925133"
		"5152000d55400d24221142453"
		"4245433549293292933415944"
		"4293555992154931432342145"
		"3192921332913251394195932"
		"0312152441442900d31415134"
		"0354321319535599459195532"
		"0215259423433341123933293"
		"s545954241213124535523211"
		"11295915a0001954112959251"
		"3121343G55522232445599331"
		"1915195315595455432341432"
		"391991500s951205533192341"
		"1231143425329101222245522"
		"1413992152545202513923135"
		"53331123235399s1434334234"
		"2512921424351455234399312"
		"5199949391315399991491114"
		"1421w53442941245932514414"
		"5924039215154944244129334"
		"3153043453423994449225495"
		"2114041411493995254539144"
		"1295195325433123155412432]"
		"[LOCATION eS 88 82 359 439 283 250 504 191]"
		"[SCORE eS 15 17 18 16 19 16 18 16]",
		25, 4, move_op_stay, true);

	test("[MAP fT "
		"5542000000000000000000000"
		"3999013519001922252942130"
		"3154055125004531549334520"
		"5392045595004452454399320"
		"5419000000029121142459140"
		"43310s0031935435231399390"
		"1232000092595559435395920"
		"1215921913511191535531930"
		"2234435534922154324124490"
		"1255113311299129544422430"
		"1422912344331423422345950"
		"390004933290000000000009s"
		"310221151199441215422904w"
		"5902523923995221913921090"
		"0204419091140000000053020"
		"04034210G1195144339033010"
		"0101534094239322395033030"
		"0502222034359434554035040"
		"0304245021914454334011000"
		"0903941025343459222092951"
		"0404339045525525459052123"
		"410454200d225141299013435"
		"03w9993099241925231051434"
		"0000091091594419149000012"
		"4241459059151325233514133" "]"
		"[LOCATION fT 324 130 534 552 -1 -1 299 -1]"
		"[SCORE fT 65 76 23 29 23 23 57 68]",
		25, 1, move_op_down, false);

	test("[MAP A@ "
		"12345"
		"0sdwG"
		"99999"
		"54321"
		"w2345"
		"][LOCATION A@ 20 6 7 8][SCORE 10 1 2 3]",
		5, 0, move_op_up, false);

	test("[MAP A@ "
		"w3000"
		"20000"
		"00000"
		"00000"
		"0000G"
		"][LOCATION A@ 0][SCORE 10]",
		5, 0, move_op_right, false);

	return 0;
}
