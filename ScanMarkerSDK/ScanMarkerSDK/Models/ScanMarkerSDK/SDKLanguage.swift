//
//  SDKLanguage.swift
//  ScanMarkerSDK
//
//  Created by dave bitton on 17/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

extension ScanMarkerSDK {
    //languages structure
    public enum OCRLanguage : String, CaseIterable  {
        case Afrikaans = "Africans",
        Albanian = "Albanian",
        Armenian,
        Aymara,
        AzeriCyrilic,
        AzeriLatin,
        BARCODE,
        Basque,
        Belarusian,
        Bemba,
        Brazilian,
        Breton,
        Bulgarian,
        ChineseSimplified,
        ChineseSimplifiedV,
        ChineseTraditional,
        ChineseTraditionalV,
        CMC7,
        Corsican,
        Croatian,
        Czech,
        Danish,
        Digits,
        Dutch,
        English,
        Esperanto,
        Estonian,
        Faroese,
        Fijian,
        Finnish,
        French,
        Frisian,
        Friulian,
        Galician,
        German,
        Greek,
        Hebrew,
        Hungarian,
        Icelandic,
        Irish,
        Italian,
        Japanese,
        JapaneseV,
        Kazakh,
        Korean,
        KoreanV,
        Latin,
        Latvian,
        Lithuanian,
        Luba,
        Luxembourg,
        Macedonian,
        Maltese,
        Moldavian,
        Mongolian,
        Norwegian,
        Papamiento,
        Polish,
        Portuguese,
        Quecha,
        Rhaeto,
        Romanian,
        Russian,
        ScottishGaelic,
        Serbian,
        Slovak,
        Slovenian,
        Spanish,
        Swahili,
        Swedish,
        Tagalog,
        Tatar,
        Turkish,
        Turkmen,
        Ukrainian,
        Uzbek,
        Vietnamese,
        Welsh,
        Xhosa,
        Zulu
        
        var codeValue : Int {
            switch self {
            case .Afrikaans: return 271
            case .Albanian: return  265
            case .Armenian: return  363
            case .Aymara: return  280
            case .AzeriCyrilic: return  345
            case .AzeriLatin: return  345
            case .BARCODE: return  370
            case .Basque: return  269
            case .Belarusian: return  259
            case .Bemba: return  289
            case .Brazilian: return  237
            case .Breton: return  270
            case .Bulgarian: return  261
            case .ChineseSimplified: return  222
            case .ChineseSimplifiedV: return  222
            case .ChineseTraditional: return  221
            case .ChineseTraditionalV: return  221
            case .CMC7: return  332
            case .Corsican: return  271
            case .Croatian: return  255
            case .Czech: return  252
            case .Danish: return  223
            case .Digits: return  200
            case .Dutch: return  224
            case .English: return  220
            case .Esperanto: return  340
            case .Estonian: return  262
            case .Faroese: return  281
            case .Fijian: return  291
            case .Finnish: return  226
            case .French: return  227
            case .Frisian: return  272
            case .Friulian: return  282
            case .Galician: return  250
            case .German: return  357
            case .Greek: return  229
            case .Hebrew: return  230
            case .Hungarian: return  253
            case .Icelandic: return  251
            case .Irish: return  267
            case .Italian: return  231
            case .Japanese: return  278
            case .JapaneseV: return  278
            case .Kazakh: return  359
            case .Korean: return  232
            case .KoreanV: return  232
            case .Latin: return  330
            case .Latvian: return  233
            case .Lithuanian: return  263
            case .Luba : return  346
            case .Luxembourg: return  258
            case .Macedonian: return  260
            case .Maltese: return  341
            case .Moldavian: return  356
            case .Mongolian: return  360
            case .Norwegian: return  234
            case .Papamiento: return  347
            case .Polish: return  235
            case .Portuguese: return  236
            case .Quecha: return  279
            case .Rhaeto: return  285
            case .Romanian: return  238
            case .Russian: return  239
            case .ScottishGaelic: return  268
            case .Serbian: return  256
            case .Slovak: return  254
            case .Slovenian: return  257
            case .Spanish: return  240
            case .Swahili : return  276
            case .Swedish: return  241
            case .Tagalog: return  277
            case .Tatar: return  348
            case .Turkish: return  242
            case .Turkmen: return  349
            case .Ukrainian: return  243
            case .Uzbek: return  361
            case .Vietnamese: return  373
            case .Welsh: return  350
            case .Xhosa: return  314
            case .Zulu: return  342
            }
        }
        func isVerticalLanguage() -> Bool {
            return self.codeValue == 222 ||
                self.codeValue == 221 ||
                self.codeValue == 278 ||
                self.codeValue == 232
        }
        
        public static func allValues() -> [String]{
            return OCRLanguage.allCases.map {$0.rawValue}
        }
    }
}
