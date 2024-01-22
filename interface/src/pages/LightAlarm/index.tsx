export function LightAlarm() {
  return (
    <div class="container py-8 flex flex-col justify-center items-center">
      <h1>Light Alarm</h1>
      <button
        className="btn"
        onClick={() => (document.getElementById("modal") as HTMLDialogElement).showModal()}
      >
        open modal
      </button>
      <dialog id="modal" className="modal modal-bottom sm:modal-middle">
        <div className="modal-box">
          <form method="dialog">
            {/* if there is a button in form, it will close the modal */}
            <button className="btn btn-sm btn-circle btn-ghost absolute right-2 top-2">✕</button>
          </form>
          <h3 className="font-bold text-lg">Hello!</h3>
          <p className="py-4">Press ESC key or click outside to close</p>
        </div>
        <form method="dialog" className="modal-backdrop">
          <button>close</button>
        </form>
      </dialog>
    </div>
  );
}
