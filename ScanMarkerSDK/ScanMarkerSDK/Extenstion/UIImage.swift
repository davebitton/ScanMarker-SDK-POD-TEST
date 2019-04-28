//
//  UIImage.swift
//  ScanMarkerSDK
//
//  Created by dave bitton on 22/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

import Foundation

extension UIImage {
    func splitToThird() -> UIImage? {
        let thirdImgRef = self.cgImage?.cropping(to: CGRect(x: 0, y: 0, width: self.size.width / 3.0, height: self.size.height / 3.0))
        var thirdImage: UIImage? = nil
        if let thirdImgRef = thirdImgRef {
            thirdImage = UIImage(cgImage: thirdImgRef)
        }
        return thirdImage
    }
}
