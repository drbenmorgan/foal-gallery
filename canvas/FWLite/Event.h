#ifndef art_FWLite_Event_h
#define art_FWLite_Event_h

// Main interface to users. It uses the DataGetterHelper
// and EventNavigator to iterate over events in a set
// of input files and find products in them.

#include "canvas/FWLite/InputTag.h"
#include "canvas/FWLite/ValidHandle.h"
#include "canvas/Persistency/Common/Wrapper.h"

#include "canvas/Persistency/Provenance/EventAuxiliary.h"
#include "canvas/Persistency/Provenance/History.h"
#include "canvas/Persistency/Provenance/ProcessHistory.h"
#include "canvas/Persistency/Provenance/ProcessHistoryID.h"

#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

class TFile;
class TTree;

namespace canvas {

  class DataGetterHelper;
  class EventNavigator;

  class Event {
  public:

    Event(std::vector<std::string> const& fileNames,
          bool useTTreeCache = true,
          unsigned int eventsToLearnUsedBranches = 7);

    ~Event();

    template <typename PROD>
    canvas::ValidHandle<PROD>
    getValidHandle(canvas::InputTag const&) const;

    art::EventAuxiliary const& eventAuxiliary() const;
    art::History const& history() const;
    art::ProcessHistoryID const& processHistoryID() const;
    art::ProcessHistory const& processHistory() const;

    long long numberOfEventsInFile() const;
    long long eventEntry() const;
    long long fileEntry() const;

    bool isValid() const;
    bool atEnd() const;

    void toBegin();
    Event& operator++();
    void next();

    TFile* getTFile() const;
    TTree* getTTree() const;

  private:

    Event(Event const&) = delete;
    Event const& operator=(Event const&) = delete;

    void getByLabel(std::type_info const& typeInfoOfWrapper,
                    InputTag const& inputTag,
                    void* ptrToPtrToWrapper) const;

    void throwProductNotFoundException(std::type_info const& typeInfo,
                                       InputTag const& tag) const;
    void checkForEnd() const;
    void updateAfterEventChange(long long oldFileEntry);

    std::unique_ptr<EventNavigator> eventNavigator_;
    std::unique_ptr<DataGetterHelper> dataGetterHelper_;

    bool useTTreeCache_;
    unsigned int eventsToLearnUsedBranches_;
    unsigned int eventsProcessed_;
  };

  template <typename PROD>
  ValidHandle<PROD>
  Event::getValidHandle(InputTag const& inputTag) const {
    checkForEnd();
    std::type_info const& typeInfoOfWrapper = typeid(art::Wrapper<PROD>);
    art::Wrapper<PROD>* ptrToWrapper;
    void* ptrToPtrToWrapper = &ptrToWrapper;

    getByLabel(typeInfoOfWrapper,
               inputTag,
               ptrToPtrToWrapper);

    if(ptrToWrapper == nullptr) {
      throwProductNotFoundException(typeid(PROD), inputTag);
    }
    PROD const* product = ptrToWrapper->product();
    return ValidHandle<PROD>(product);
  }
}

#endif /* art_FWLite_Event_h */

// Local Variables:
// mode: c++
// End:
