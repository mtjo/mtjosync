#ifndef NOTIFIER_H_
#define NOTIFIER_H_
#include "MRApp.h"
#include <string>
class Notifier: public MRApp {
public:
  Notifier();
  virtual ~Notifier();

  virtual void onLaunched(const std::vector<std::string>& parameters);
};

#endif /* NOTIFIER_H_ */
