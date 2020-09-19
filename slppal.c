#include "aokbase.h"

/* slppal pal_in.pal pal_out.pal slp_in.slp slp_out.slp */

uchar colmap[256];

char pal_aoe[] = "JASC-PAL\n0100\n256\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n163 0 255\n195 0 255\n227 0 255\n255 0 243\n255 0 210\n255 0 178\n227 247 255\n187 215 235\n147 187 215\n115 155 199\n87 123 179\n63 95 159\n39 63 143\n23 39 123\n7 15 103\n0 0 87\n255 255 243\n255 255 119\n255 255 79\n255 255 39\n254 254 0\n199 199 0\n255 239 239\n255 191 191\n255 143 143\n255 95 95\n255 47 47\n227 11 0\n199 23 0\n143 31 0\n111 11 7\n83 11 0\n171 171 0\n147 147 0\n119 119 0\n91 91 0\n63 63 0\n35 35 0\n255 255 199\n255 255 159\n255 255 0\n227 227 0\n223 207 15\n195 163 27\n163 115 23\n135 103 39\n107 75 39\n79 55 35\n11 11 0\n243 255 243\n211 255 191\n207 255 143\n155 255 95\n87 255 47\n243 219 187\n231 195 115\n207 163 67\n183 139 43\n163 115 79\n139 91 55\n115 71 39\n95 51 27\n63 55 35\n35 35 31\n0 255 0\n0 231 23\n0 211 39\n0 187 55\n0 167 67\n0 143 71\n255 219 179\n255 195 111\n251 159 31\n247 139 23\n243 119 15\n239 99 7\n207 67 0\n159 51 0\n135 43 0\n111 35 0\n0 123 75\n0 103 99\n0 55 83\n0 19 63\n0 0 43\n255 247 235\n175 207 147\n155 183 111\n139 159 79\n127 139 55\n99 123 47\n75 107 43\n55 95 39\n27 67 27\n19 51 19\n11 27 11\n247 235 215\n239 219 195\n231 207 175\n223 195 159\n215 183 143\n211 171 127\n254 254 254\n235 235 235\n219 219 219\n199 199 199\n179 179 179\n143 143 143\n107 107 107\n71 71 71\n55 55 55\n35 35 35\n187 143 99\n78 55 35\n47 47 35\n255 251 247\n235 227 219\n219 207 195\n235 255 239\n159 231 187\n95 211 159\n43 191 147\n0 171 147\n0 131 123\n0 111 107\n0 79 79\n0 63 67\n0 35 39\n199 183 175\n183 163 151\n163 143 131\n147 123 111\n127 103 95\n111 87 79\n107 79 71\n103 75 63\n99 71 59\n95 67 55\n91 63 47\n87 59 43\n83 59 39\n255 1 1\n171 31 0\n115 35 0\n99 23 0\n67 7 0\n51 0 0\n35 0 0\n243 243 255\n219 219 243\n199 199 231\n179 183 223\n159 167 211\n143 151 203\n127 131 183\n115 119 167\n99 103 151\n87 87 135\n75 75 119\n51 51 99\n31 31 83\n15 15 63\n7 7 47\n0 0 31\n255 235 215\n255 203 143\n255 183 75\n255 179 43\n235 79 0\n183 59 0\n203 231 187\n39 79 31\n15 39 15\n163 163 163\n127 127 127\n91 91 91\n15 15 15\n1 1 1\n255 239 219\n247 223 199\n239 207 179\n231 187 163\n223 171 147\n215 151 131\n207 135 115\n199 115 99\n191 99 87\n171 71 63\n151 47 39\n131 27 23\n95 0 0\n251 247 243\n247 231 215\n239 207 163\n235 199 139\n231 191 95\n139 95 11\n119 75 0\n255 0 247\n255 0 227\n255 0 211\n255 0 195\n255 0 179\n255 0 163\n255 0 147\n255 0 131\n255 0 115\n255 0 95\n255 0 79\n255 0 63\n255 0 47\n255 0 31\n255 0 15\n255 0 0\n251 251 251\n247 243 235\n247 239 227\n243 235 215\n243 231 207\n243 227 195\n243 219 183\n247 215 175\n247 207 163\n251 199 151\n255 191 143\n227 171 127\n203 151 111\n175 131 95\n151 111 79\n127 95 67\n167 131 35\n0 151 135\n0 95 95\n0 47 51\n2 2 2\n253 253 253\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n255 255 255\n";
char pal_aok[] = "JASC-PAL\n0100\n256\n0 0 0\n0 74 161\n0 97 155\n0 74 187\n0 84 176\n0 90 184\n0 110 176\n0 110 187\n0 84 197\n0 98 210\n0 0 0\n47 47 47\n67 67 67\n87 87 87\n37 16 6\n47 26 17\n0 0 82\n0 21 130\n19 49 161\n48 93 182\n74 121 208\n110 166 235\n151 206 255\n205 250 255\n64 43 23\n67 51 27\n70 32 6\n75 57 42\n84 64 43\n87 69 37\n87 57 27\n94 74 48\n65 0 0\n105 11 0\n160 21 0\n230 11 0\n255 0 0\n255 100 100\n255 160 160\n255 220 220\n97 77 67\n103 58 21\n113 75 51\n113 75 13\n115 105 84\n125 97 72\n125 74 0\n129 116 95\n0 0 0\n0 7 0\n0 32 0\n0 59 0\n0 87 0\n0 114 0\n0 141 0\n0 169 0\n134 126 118\n135 64 0\n136 108 79\n144 100 12\n146 125 105\n153 106 55\n159 121 88\n166 74 0\n80 51 26\n140 78 9\n191 123 0\n255 199 0\n255 247 37\n255 255 97\n255 255 166\n255 255 227\n167 135 102\n172 144 115\n175 126 36\n175 151 128\n185 151 146\n186 166 135\n187 84 0\n187 156 125\n110 23 0\n150 36 0\n210 55 0\n255 80 0\n255 130 1\n255 180 21\n255 210 75\n255 235 160\n189 150 111\n191 169 115\n195 174 156\n196 170 146\n196 128 88\n196 166 135\n197 187 176\n204 160 36\n0 16 16\n0 37 41\n0 80 80\n0 120 115\n0 172 150\n38 223 170\n109 252 191\n186 255 222\n206 169 133\n207 105 12\n207 176 156\n208 155 67\n215 186 155\n216 162 121\n217 114 24\n217 187 166\n47 0 46\n79 0 75\n133 12 121\n170 47 155\n211 58 201\n241 108 232\n255 169 255\n255 210 255\n218 156 105\n222 177 136\n225 177 90\n226 195 170\n232 180 120\n235 202 181\n235 216 190\n237 199 165\n28 28 28\n67 67 67\n106 106 106\n145 145 145\n185 185 185\n223 223 223\n255 255 255\n255 255 255\n247 211 191\n248 201 138\n255 206 157\n255 225 201\n255 238 217\n255 226 161\n255 243 220\n255 255 243\n21 21 0\n37 37 17\n27 47 0\n47 57 17\n67 77 7\n77 77 47\n44 77 3\n94 84 53\n95 97 39\n97 97 67\n67 97 29\n106 115 57\n116 115 75\n87 116 7\n118 130 65\n130 136 77\n138 139 87\n148 155 100\n156 156 139\n128 157 84\n149 166 97\n175 165 106\n176 176 159\n146 176 67\n194 190 148\n165 196 108\n166 196 77\n206 187 128\n206 204 155\n204 217 77\n221 218 166\n196 226 116\n247 204 17\n3 28 0\n7 38 0\n7 47 7\n19 48 0\n27 57 17\n47 57 47\n28 62 0\n14 68 14\n41 69 28\n33 73 18\n47 87 47\n77 97 57\n67 97 67\n87 116 77\n70 119 48\n85 119 52\n106 136 97\n196 236 166\n23 53 33\n43 84 64\n37 116 57\n23 43 53\n2 33 53\n2 23 53\n33 64 64\n0 34 97\n0 51 115\n43 64 74\n0 43 74\n4 6 9\n0 123 189\n64 84 84\n0 115 207\n23 23 74\n12 23 64\n0 0 2\n0 64 125\n2 23 84\n0 138 186\n64 105 105\n0 146 197\n94 105 105\n0 74 125\n0 125 207\n0 120 227\n84 115 125\n64 105 125\n0 64 146\n0 53 135\n115 156 156\n84 146 176\n146 176 187\n207 248 255\n105 166 197\n125 197 217\n156 197 217\n109 126 33\n113 153 36\n21 118 21\n51 151 39\n70 181 59\n89 223 89\n131 245 120\n174 255 174\n0 255 0\n0 0 255\n255 255 0\n255 213 0\n226 154 73\n241 164 82\n255 171 88\n255 197 113\n85 125 57\n129 151 49\n0 255 255\n255 0 255\n0 139 210\n0 160 243\n255 255 255\n";
char pal_aoc[] = "JASC-PAL\n0100\n256\n0 0 0\n0 74 161\n0 97 155\n0 74 187\n0 84 176\n0 90 184\n0 110 176\n0 110 187\n180 255 180\n0 98 210\n0 0 0\n47 47 47\n200 216 255\n87 87 87\n37 16 6\n47 26 17\n0 0 82\n0 21 130\n19 49 161\n48 93 182\n74 121 208\n110 166 235\n151 206 255\n205 250 255\n64 43 23\n67 51 27\n70 32 6\n75 57 42\n84 64 43\n87 69 37\n87 57 27\n94 74 48\n65 0 0\n105 11 0\n160 21 0\n230 11 0\n255 0 0\n255 100 100\n255 160 160\n255 220 220\n97 77 67\n103 58 21\n113 75 51\n113 75 13\n115 105 84\n125 97 72\n125 74 0\n129 116 95\n0 0 0\n0 7 0\n0 32 0\n0 59 0\n0 87 0\n0 114 0\n0 141 0\n0 169 0\n134 126 118\n135 64 0\n136 108 79\n144 100 12\n146 125 105\n153 106 55\n159 121 88\n166 74 0\n80 51 26\n140 78 9\n191 123 0\n255 199 0\n255 247 37\n255 255 97\n255 255 166\n255 255 227\n167 135 102\n172 144 115\n175 126 36\n175 151 128\n185 151 146\n186 166 135\n187 84 0\n187 156 125\n110 23 0\n150 36 0\n210 55 0\n255 80 0\n255 130 1\n255 180 21\n255 210 75\n255 235 160\n189 150 111\n191 169 115\n195 174 156\n196 170 146\n196 128 88\n196 166 135\n197 187 176\n204 160 36\n0 16 16\n0 37 41\n0 80 80\n0 120 115\n0 172 150\n38 223 170\n109 252 191\n186 255 222\n206 169 133\n207 105 12\n207 176 156\n228 162 82\n215 186 155\n216 162 121\n217 114 24\n223 234 255\n47 0 46\n79 0 75\n133 12 121\n170 47 155\n211 58 201\n241 108 232\n255 169 255\n255 210 255\n218 156 105\n222 177 136\n225 177 90\n226 195 170\n232 180 120\n235 202 181\n235 216 190\n237 199 165\n28 28 28\n67 67 67\n106 106 106\n145 145 145\n185 185 185\n223 223 223\n247 247 247\n255 255 255\n247 211 191\n248 201 138\n255 206 157\n255 225 201\n255 238 217\n255 226 161\n216 223 255\n255 255 243\n24 24 0\n37 37 17\n27 47 0\n47 57 17\n67 77 7\n77 77 47\n44 77 3\n94 84 53\n95 97 39\n97 97 67\n67 97 29\n106 115 57\n116 115 75\n87 116 7\n118 130 65\n130 136 77\n138 139 87\n148 155 100\n156 156 139\n128 157 84\n149 166 97\n175 165 106\n176 176 159\n146 176 67\n194 190 148\n165 196 108\n166 196 77\n206 187 128\n206 204 155\n204 217 77\n221 218 166\n196 226 116\n243 170 92\n3 28 0\n7 38 0\n7 47 7\n19 48 0\n27 57 17\n47 57 47\n28 62 0\n14 68 14\n41 69 28\n33 73 18\n47 87 47\n77 97 57\n67 97 67\n87 116 77\n70 119 48\n189 209 253\n106 136 97\n196 236 166\n23 53 33\n43 84 64\n37 116 57\n23 43 53\n2 33 53\n2 23 53\n33 64 64\n0 34 97\n0 51 115\n43 64 74\n0 43 74\n4 6 9\n0 123 189\n64 84 84\n0 115 207\n23 23 74\n12 23 64\n255 177 98\n0 64 125\n2 23 84\n0 138 186\n64 105 105\n0 146 197\n94 105 105\n0 74 125\n0 125 207\n95 133 65\n84 115 125\n64 105 125\n0 64 146\n0 53 135\n115 156 156\n84 146 176\n146 176 187\n255 201 121\n105 166 197\n125 197 217\n156 197 217\n109 126 33\n113 153 36\n21 118 21\n51 151 39\n70 181 59\n89 223 89\n131 245 120\n152 192 240\n0 255 0\n0 0 255\n255 255 0\n255 217 0\n240 240 255\n240 247 255\n247 247 255\n247 255 255\n85 119 52\n129 151 49\n0 255 255\n255 0 255\n0 139 210\n0 160 243\n255 255 255\n";
char pal_aok_alpha_1[] = "JASC-PAL\n0100\n256\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n202 168 27\n175 144 28\n149 120 29\n122 96 30\n96 73 31\n69 49 32\n69 5 104\n93 7 140\n116 6 177\n139 3 214\n177 38 255\n186 61 255\n206 117 255\n226 171 255\n205 153 114\n214 165 125\n221 176 136\n229 189 148\n235 201 160\n240 212 175\n245 222 188\n249 232 204\n0 0 87\n11 19 117\n23 39 147\n34 58 177\n82 105 197\n131 153 216\n179 200 236\n227 247 255\n2 77 2\n2 95 2\n1 112 2\n1 130 2\n1 147 2\n1 165 2\n0 182 2\n0 200 2\n74 3 3\n100 17 15\n123 32 28\n151 47 39\n167 73 62\n185 103 90\n201 129 112\n217 155 136\n139 159 79\n121 142 70\n104 125 61\n86 108 52\n68 90 42\n50 73 33\n33 56 24\n15 39 15\n51 0 0\n88 6 0\n127 14 2\n171 20 4\n205 28 4\n220 65 53\n237 104 98\n255 142 143\n255 255 137\n255 255 93\n255 255 49\n255 255 0\n248 239 7\n242 223 13\n235 207 20\n228 191 26\n63 46 32\n81 56 36\n99 64 38\n116 73 41\n144 105 73\n175 141 110\n204 176 145\n233 210 180\n247 231 215\n238 218 190\n229 205 166\n220 192 141\n210 178 117\n201 165 92\n192 152 68\n183 139 43\n15 39 15\n23 52 20\n31 66 26\n39 79 31\n68 105 51\n104 139 83\n139 173 115\n175 207 147\n2 81 51\n23 94 54\n44 107 57\n65 120 60\n87 133 63\n108 146 66\n129 159 69\n150 172 72\n171 185 75\n183 195 76\n195 205 76\n207 215 77\n219 225 77\n231 235 78\n243 245 78\n204 152 113\n205 139 90\n207 129 64\n203 116 44\n190 108 33\n177 97 24\n162 85 15\n144 77 9\n125 65 4\n0 0 0\n34 34 34\n68 68 68\n102 102 102\n136 136 136\n170 170 170\n204 204 204\n238 238 238\n17 17 17\n51 51 51\n85 85 85\n119 119 119\n153 153 153\n187 187 187\n221 221 221\n254 254 254\n255 255 0\n253 233 0\n250 211 0\n248 189 0\n245 167 0\n243 145 0\n240 123 0\n238 101 0\n235 79 0\n219 59 0\n203 40 0\n186 20 0\n170 0 0\n141 4 0\n112 7 0\n83 11 0\n0 35 39\n0 55 58\n0 75 76\n0 95 95\n40 129 118\n80 163 141\n119 197 164\n159 231 187\n19 51 29\n46 70 43\n73 88 57\n100 107 70\n127 125 84\n154 144 98\n181 162 112\n208 181 125\n235 199 139\n221 184 122\n206 168 104\n192 153 87\n177 137 70\n163 122 52\n148 106 35\n134 91 17\n119 75 0\n111 69 3\n103 64 5\n95 58 8\n88 52 11\n80 46 14\n72 41 16\n64 35 19\n111 35 0\n152 50 0\n194 64 0\n235 79 0\n240 110 36\n245 141 72\n250 172 107\n255 203 143\n219 219 243\n199 200 228\n179 180 212\n159 161 197\n139 142 182\n119 122 166\n99 103 151\n89 92 140\n78 81 129\n68 70 118\n57 59 107\n47 48 96\n36 37 85\n26 26 74\n15 15 63\n24 37 74\n33 59 84\n42 80 95\n53 104 108\n61 126 118\n72 151 132\n82 173 147\n93 192 157\n104 215 170\n73 73 2\n112 112 2\n150 150 1\n189 189 1\n227 227 0\n236 236 53\n246 246 106\n255 255 159\n83 59 39\n98 75 56\n112 90 73\n127 106 90\n141 121 107\n156 137 124\n170 152 141\n185 168 158\n170 149 136\n155 129 114\n140 110 93\n125 90 71\n110 71 49\n95 51 27\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n0 0 0\n205 2 200\n218 2 214\n230 1 228\n243 1 241\n255 0 255\n";
char pal_aok_alpha_2[] = "JASC-PAL\n0100\n256\n0 0 0\n23 45 53\n25 50 83\n25 68 73\n24 68 99\n56 83 78\n38 77 121\n69 92 99\n60 90 121\n47 107 115\n0 0 0\n52 52 52\n68 68 68\n84 84 84\n44 27 19\n52 35 28\n0 6 80\n13 31 119\n30 54 144\n53 89 161\n74 112 182\n103 148 203\n136 180 218\n179 215 233\n66 49 33\n68 55 36\n71 40 19\n75 60 48\n82 66 49\n84 70 44\n84 60 36\n90 74 53\n67 7 0\n99 23 0\n143 31 0\n199 23 0\n255 0 0\n255 95 95\n255 143 143\n255 191 191\n92 76 68\n97 61 31\n105 75 55\n105 75 25\n107 99 82\n115 92 72\n115 74 0\n118 108 91\n2 2 2\n2 20 2\n2 40 2\n2 62 2\n2 84 2\n2 106 2\n2 128 2\n2 150 2\n122 116 109\n123 66 0\n124 101 78\n130 95 24\n132 115 99\n137 100 59\n142 112 85\n148 74 8\n79 55 35\n127 77 22\n168 113 13\n218 174 13\n255 213 44\n255 230 92\n255 239 148\n255 255 197\n149 123 96\n153 130 107\n155 116 43\n155 136 117\n163 136 132\n164 148 123\n165 82 8\n165 140 115\n103 33 2\n135 43 0\n183 59 0\n235 79 0\n243 119 15\n251 159 31\n251 183 75\n255 203 143\n166 135 104\n168 150 107\n171 154 140\n172 151 132\n172 117 85\n172 148 123\n173 165 156\n178 143 43\n6 27 27\n2 44 47\n0 79 79\n0 111 107\n0 153 135\n45 194 151\n102 217 168\n164 235 193\n180 150 121\n181 99 24\n181 156 140\n182 139 68\n187 164 139\n188 145 112\n189 106 34\n189 165 148\n52 3 51\n78 7 75\n121 24 112\n151 52 139\n184 61 176\n208 101 201\n225 150 221\n236 183 234\n190 140 99\n193 157 124\n195 157 87\n196 171 151\n201 159 111\n203 177 160\n203 188 167\n205 174 147\n37 37 37\n68 68 68\n100 100 100\n131 131 131\n163 163 163\n194 194 194\n226 226 226\n255 255 255\n213 184 168\n214 176 125\n219 180 141\n221 195 176\n222 206 189\n228 196 144\n239 210 191\n251 225 210\n31 31 0\n44 44 28\n36 52 12\n52 60 28\n68 76 20\n76 76 52\n50 76 17\n90 82 57\n91 92 46\n92 92 68\n68 92 38\n100 107 60\n108 107 75\n84 108 20\n109 119 67\n119 124 76\n125 126 84\n133 139 95\n140 140 126\n117 141 82\n134 148 92\n155 147 100\n156 156 142\n132 156 68\n170 167 133\n147 172 101\n148 172 76\n180 165 117\n180 178 139\n178 189 76\n192 190 148\n172 196 108\n213 178 28\n17 37 5\n20 45 8\n20 52 20\n30 53 12\n36 60 28\n52 60 52\n37 64 14\n26 69 26\n47 70 37\n41 73 29\n52 84 52\n76 92 60\n68 92 68\n84 108 76\n71 110 53\n83 110 56\n100 124 92\n172 204 148\n33 57 41\n49 82 66\n44 108 60\n33 49 57\n16 41 57\n16 33 57\n41 66 66\n0 49 66\n24 49 66\n49 66 74\n0 49 74\n18 19 22\n33 82 82\n66 82 82\n49 82 82\n33 33 74\n24 33 66\n0 0 16\n33 66 82\n16 33 82\n33 82 90\n66 99 99\n49 99 99\n90 99 99\n49 82 99\n66 115 115\n49 99 115\n82 107 115\n66 99 115\n148 189 214\n66 115 123\n107 140 140\n82 132 156\n132 156 165\n181 214 231\n99 148 173\n115 173 189\n140 173 189\n102 116 41\n105 137 43\n31 109 31\n55 136 46\n71 160 62\n86 194 86\n120 211 111\n154 234 154\n0 255 0\n0 0 255\n255 255 0\n255 186 1\n196 138 73\n208 146 80\n219 152 85\n230 173 105\n83 115 60\n118 136 54\n0 255 255\n255 0 255\n79 111 110\n85 133 161\n255 0 255\n";
char pal_aok_beta[] = "JASC-PAL\n0100\n256\n0 0 0\n0 74 144\n0 92 139\n0 74 165\n0 82 156\n0 87 162\n0 103 156\n0 103 165\n0 82 173\n0 93 183\n0 0 0\n52 52 52\n68 68 68\n84 84 84\n44 27 19\n52 35 28\n0 6 80\n13 31 119\n30 54 144\n53 89 161\n74 112 182\n103 148 203\n136 180 218\n179 215 233\n66 49 33\n68 55 36\n71 40 19\n75 60 48\n82 66 49\n84 70 44\n84 60 36\n90 74 53\n67 7 0\n99 23 0\n143 31 0\n199 23 0\n255 0 0\n255 95 95\n255 143 143\n255 191 191\n92 76 68\n97 61 31\n105 75 55\n105 75 25\n107 99 82\n115 92 72\n115 74 0\n118 108 91\n2 2 2\n2 20 2\n2 40 2\n2 62 2\n2 84 2\n2 106 2\n2 128 2\n2 150 2\n122 116 109\n123 66 0\n124 101 78\n130 95 24\n132 115 99\n137 100 59\n142 112 85\n148 74 8\n79 55 35\n127 77 22\n168 113 13\n218 174 13\n255 213 44\n255 230 92\n255 239 148\n255 255 197\n149 123 96\n153 130 107\n155 116 43\n155 136 117\n163 136 132\n164 148 123\n165 82 8\n165 140 115\n103 33 2\n135 43 0\n183 59 0\n235 79 0\n243 119 15\n251 159 31\n251 183 75\n255 203 143\n166 135 104\n168 150 107\n171 154 140\n172 151 132\n172 117 85\n172 148 123\n173 165 156\n178 143 43\n6 27 27\n2 44 47\n0 79 79\n0 111 107\n0 153 135\n45 194 151\n102 217 168\n164 235 193\n180 150 121\n181 99 24\n181 156 140\n182 139 68\n187 164 139\n188 145 112\n189 106 34\n189 165 148\n52 3 51\n78 7 75\n121 24 112\n151 52 139\n184 61 176\n208 101 201\n225 150 221\n236 183 234\n190 140 99\n193 157 124\n195 157 87\n196 171 151\n201 159 111\n203 177 160\n203 188 167\n205 174 147\n37 37 37\n68 68 68\n100 100 100\n131 131 131\n163 163 163\n194 194 194\n226 226 226\n255 255 255\n213 184 168\n214 176 125\n219 180 141\n221 195 176\n222 206 189\n228 196 144\n239 210 191\n251 225 210\n31 31 0\n44 44 28\n36 52 12\n52 60 28\n68 76 20\n76 76 52\n50 76 17\n90 82 57\n91 92 46\n92 92 68\n68 92 38\n100 107 60\n108 107 75\n84 108 20\n109 119 67\n119 124 76\n125 126 84\n133 139 95\n140 140 126\n117 141 82\n134 148 92\n155 147 100\n156 156 142\n132 156 68\n170 167 133\n147 172 101\n148 172 76\n180 165 117\n180 178 139\n178 189 76\n192 190 148\n172 196 108\n213 178 28\n17 37 5\n20 45 8\n20 52 20\n30 53 12\n36 60 28\n52 60 52\n37 64 14\n26 69 26\n47 70 37\n41 73 29\n52 84 52\n76 92 60\n68 92 68\n84 108 76\n71 110 53\n83 110 56\n100 124 92\n172 204 148\n33 57 41\n49 82 66\n44 108 60\n33 49 57\n16 41 57\n16 33 57\n41 66 66\n0 42 92\n0 55 107\n49 66 74\n0 49 74\n18 19 22\n0 113 166\n66 82 82\n0 107 181\n33 33 74\n24 33 66\n0 0 16\n0 66 115\n16 33 82\n0 125 164\n66 99 99\n0 132 173\n90 99 99\n0 74 115\n0 115 181\n0 111 197\n82 107 115\n66 99 115\n0 66 132\n0 57 123\n107 140 140\n82 132 156\n132 156 165\n181 214 231\n99 148 173\n115 173 189\n140 173 189\n102 116 41\n105 137 43\n31 109 31\n55 136 46\n71 160 62\n86 194 86\n120 211 111\n154 234 154\n0 255 0\n0 0 255\n255 255 0\n255 186 1\n196 138 73\n208 146 80\n219 152 85\n230 173 105\n83 115 60\n118 136 54\n0 255 255\n255 0 255\n0 126 183\n0 143 210\n255 0 255\n";
char pal_aoc_beta[] = "JASC-PAL\n0100\n256\n0 0 0\n0 74 144\n0 92 139\n0 74 165\n0 82 156\n0 87 162\n0 103 156\n0 103 165\n180 255 180\n0 93 183\n0 0 0\n52 52 52\n200 216 255\n84 84 84\n44 27 19\n52 35 28\n0 6 80\n13 31 119\n30 54 144\n53 89 161\n74 112 182\n103 148 203\n136 180 218\n179 215 233\n66 49 33\n68 55 36\n71 40 19\n75 60 48\n82 66 49\n84 70 44\n84 60 36\n90 74 53\n67 7 0\n99 23 0\n143 31 0\n199 23 0\n255 0 0\n255 95 95\n255 143 143\n255 191 191\n92 76 68\n97 61 31\n105 75 55\n105 75 25\n107 99 82\n115 92 72\n115 74 0\n118 108 91\n2 2 2\n2 20 2\n2 40 2\n2 62 2\n2 84 2\n2 106 2\n2 128 2\n2 150 2\n122 116 109\n123 66 0\n124 101 78\n130 95 24\n132 115 99\n137 100 59\n142 112 85\n148 74 8\n79 55 35\n127 77 22\n168 113 13\n218 174 13\n255 213 44\n255 230 92\n255 239 148\n255 255 197\n149 123 96\n153 130 107\n155 116 43\n155 136 117\n163 136 132\n164 148 123\n165 82 8\n165 140 115\n103 33 2\n135 43 0\n183 59 0\n235 79 0\n243 119 15\n251 159 31\n251 183 75\n255 203 143\n166 135 104\n168 150 107\n171 154 140\n172 151 132\n172 117 85\n172 148 123\n173 165 156\n178 143 43\n6 27 27\n2 44 47\n0 79 79\n0 111 107\n0 153 135\n45 194 151\n102 217 168\n164 235 193\n180 150 121\n181 99 24\n181 156 140\n228 162 82\n187 164 139\n188 145 112\n189 106 34\n223 234 255\n52 3 51\n78 7 75\n121 24 112\n151 52 139\n184 61 176\n208 101 201\n225 150 221\n236 183 234\n190 140 99\n193 157 124\n195 157 87\n196 171 151\n201 159 111\n203 177 160\n203 188 167\n205 174 147\n37 37 37\n68 68 68\n100 100 100\n131 131 131\n163 163 163\n194 194 194\n226 226 226\n255 255 255\n213 184 168\n214 176 125\n219 180 141\n221 195 176\n222 206 189\n228 196 144\n216 223 255\n251 225 210\n31 31 0\n44 44 28\n36 52 12\n52 60 28\n68 76 20\n76 76 52\n50 76 17\n90 82 57\n91 92 46\n92 92 68\n68 92 38\n100 107 60\n108 107 75\n84 108 20\n109 119 67\n119 124 76\n125 126 84\n133 139 95\n140 140 126\n117 141 82\n134 148 92\n155 147 100\n156 156 142\n132 156 68\n170 167 133\n147 172 101\n148 172 76\n180 165 117\n180 178 139\n178 189 76\n192 190 148\n172 196 108\n243 170 92\n17 37 5\n20 45 8\n20 52 20\n30 53 12\n36 60 28\n52 60 52\n37 64 14\n26 69 26\n47 70 37\n41 73 29\n52 84 52\n76 92 60\n68 92 68\n84 108 76\n71 110 53\n189 209 253\n100 124 92\n172 204 148\n33 57 41\n49 82 66\n44 108 60\n33 49 57\n16 41 57\n16 33 57\n41 66 66\n0 42 92\n0 55 107\n49 66 74\n0 49 74\n18 19 22\n0 113 166\n66 82 82\n0 107 181\n33 33 74\n24 33 66\n255 177 98\n0 66 115\n16 33 82\n0 125 164\n66 99 99\n0 132 173\n90 99 99\n0 74 115\n0 115 181\n95 133 65\n82 107 115\n66 99 115\n0 66 132\n0 57 123\n107 140 140\n82 132 156\n132 156 165\n255 201 121\n99 148 173\n115 173 189\n140 173 189\n102 116 41\n105 137 43\n31 109 31\n55 136 46\n71 160 62\n86 194 86\n120 211 111\n152 192 240\n0 255 0\n0 0 255\n255 255 0\n255 186 1\n240 240 255\n240 247 255\n247 247 255\n247 255 255\n85 119 52\n118 136 54\n0 255 255\n255 0 255\n0 126 183\n0 143 210\n255 0 255\n";

void
PaletteGetFromId(char *Id, Palette *pal)
{
	if(strcmp(Id, "aoe") == 0) {
		PaletteReadJASCFromString(pal, pal_aoe);
	} else if(strcmp(Id, "aok") == 0) {
		PaletteReadJASCFromString(pal, pal_aok);
	} else if(strcmp(Id, "aoc") == 0) {
		PaletteReadJASCFromString(pal, pal_aoc);
	} else if(strcmp(Id, "aok_alpha_1") == 0) {
		PaletteReadJASCFromString(pal, pal_aok_alpha_1);
	} else if(strcmp(Id, "aok_alpha_2") == 0) {
		PaletteReadJASCFromString(pal, pal_aok_alpha_2);
	} else if(strcmp(Id, "aok_beta") == 0) {
		PaletteReadJASCFromString(pal, pal_aok_beta);
	} else if(strcmp(Id, "aoc_beta") == 0) {
		PaletteReadJASCFromString(pal, pal_aoc_beta);
	} else {
		PaletteReadJASCFromFile(pal, Id);
	}
}

uchar
colmapCB(uchar col)
{
	return colmap[col];
}

int
main(int argc, char *argv[])
{
	int i;
	FILE *f;
	Palette pal_in, pal_out;
	Shape *slp;
	uchar *data;
	int size;

	setbuf(stdout, NULL);

	if(argc < 5) {
        printf("slppal pal_in pal_out slp_in.slp slp_out.slp\n\n");
        printf("pal_in/out can be aoe, aok, aoc, aok_alpha_1, aok_alpha_2, aok_beta, aoc_beta\n");
        printf("OR a custom palette file\n");
		return 1;
    }

    PaletteGetFromId(argv[1], &pal_in);
    PaletteGetFromId(argv[2], &pal_out);

	data = getFileContents(argv[3], &size);
	slp = ShapeCreate(data);
	slp->size = size;

	f = mustopen(argv[4], "wb");

	for(i = 0; i < 256; i++)
		colmap[i] = PaletteFindColor(&pal_out, pal_in.col[i]);
	ShapeForallPixels(slp, colmapCB);

	fwrite(slp->header, 1, slp->size, f);
	fclose(f);

	return 0;
}
