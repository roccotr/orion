#include "CoreOdbb.hpp"
#include "CoreTutl.hpp"
#include "CoreCaco.hpp"
#include <glog/logging.h>
#include <gflags/gflags.h>


using namespace google;

static PTR_POOL cVpool;
static PTR_ORIONCONNECTIONFACTORY cVorionconnectionfactory;
static PTR_ARIESCONNECTION cVariesconnectionfactory;
static CoreSmph cVbuildmutex;
static int iVreferences = 0;