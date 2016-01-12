#ifndef art_FWLite_EventHistoryGetter_h
#define art_FWLite_EventHistoryGetter_h

#include "canvas/FWLite/HistoryGetterBase.h"

namespace canvas {

  class EventNavigator;

  class EventHistoryGetter : public HistoryGetterBase {
  public:

    EventHistoryGetter(EventNavigator const*);
    virtual ~EventHistoryGetter();

    EventHistoryGetter(EventHistoryGetter const&) = delete;
    EventHistoryGetter const& operator=(EventHistoryGetter const&) = delete;

    virtual art::ProcessHistoryID const& processHistoryID() const override;
    virtual art::ProcessHistory const& processHistory() const override;
    virtual art::History const& history() const override;

  private:

    EventNavigator const* eventNavigator_;
  };
}
#endif

// Local Variables:
// mode: c++
// End:
