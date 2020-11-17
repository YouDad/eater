#include "modules/algorithm.h"
#include "utils/assert.h"

/*
	printf("%d, %d\n", result.first, result.second); \
 */
#define test(map, size, id, move, is_fire) {\
	class server_data data((char *)map, size, id);\
	auto result = algorithm(data);\
	assert(result.first == move);\
	assert(result.second == is_fire);\
}

int main() {
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
		25, 4, move_op_up, false);

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
		"][LOCATION A@ 20 6 7 8][SCORE 10 1 2 3]", 5, 0, move_op_up, false);

	test("[MAP A@ "
		"w3000"
		"20000"
		"00000"
		"00000"
		"0000G"
		"][LOCATION A@ 0][SCORE 10]", 5, 0, move_op_right, false);

	return 0;
}
