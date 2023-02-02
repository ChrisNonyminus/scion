#ifndef BLOBS_FRAMEWORK_INCLUDE_CGZGRAPHICSYSTEM_H
#define BLOBS_FRAMEWORK_INCLUDE_CGZGRAPHICSYSTEM_H

struct StartupDisplayMetrics {
  int width, height, bpp, field_C, field_10, field_14, field_18;
};

class cGZGraphicSystem {
public:
  static StartupDisplayMetrics mDesiredStartupDisplayMetrics;
};

#endif //BLOBS_FRAMEWORK_INCLUDE_CGZGRAPHICSYSTEM_H
