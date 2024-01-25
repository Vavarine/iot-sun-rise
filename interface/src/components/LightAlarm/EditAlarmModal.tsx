import { TimeSelector } from "./TimeSelector";

interface EditAlarmModalProps {
  modalId: string;
}

export function EditAlarmModal({ modalId }: EditAlarmModalProps) {
  return (
    <dialog id={modalId} className="modal modal-bottom sm:modal-middle">
      <div className="modal-box">
        <form method="dialog">
          <button className="btn btn-sm btn-circle btn-ghost absolute right-2 top-2">✕</button>
        </form>
        <div class="flex justify-center">
          <TimeSelector />
        </div>
      </div>
      <form method="dialog" className="modal-backdrop">
        <button>close</button>
      </form>
    </dialog>
  );
}
