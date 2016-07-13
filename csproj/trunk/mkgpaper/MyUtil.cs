using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace mkgpaper {
    class MyUtil {
        
        public static float cm_to_inch(float cm) {
            return cm * 2.54f;
        }

        public static float mm_to_inch(float mm) {
            return cm_to_inch(mm / 10);
        }

        public static float inch_to_cm(float inch) {
            return inch / 2.54f;
        }

        public static float inch_to_mm(float inch) {
            return inch_to_cm(inch) * 10;
        }
    }
}
